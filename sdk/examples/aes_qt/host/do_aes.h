#ifndef DO_AES_H
#define DO_AES_H

#include "../AES/test_func.h"
#include <cstdio>

#ifndef PC_SIMULATION
#include <edge_call.h>
#include <keystone.h>

#define OCALL_GET_INPUT   1
#define OCALL_SET_OUTPUT1 2
#define OCALL_SET_OUTPUT2 3
#endif

class Aes {
public:
    static int tee_domain_aes(uint8_t *in, uint8_t *out1, uint8_t *out2)
    {
#ifndef PC_SIMULATION
        Keystone::Enclave enclave;
        Keystone::Params params;

        params.setFreeMemSize(2 * 1024 * 1024);
        params.setUntrustedMem(DEFAULT_UNTRUSTED_PTR, 2 * 1024 * 1024);

        if(enclave.init("aes_qt", "eyrie-rt", params) != Keystone::Error::Success) {
            printf("Failed to initialize enclave.\n");
            return -1;
        }

        enclave.registerOcallDispatch([&in,&out1,&out2](void* buffer) {
            if(buffer) {
                struct edge_call* edge_call = (struct edge_call*)buffer;
                uintptr_t call_args;
                unsigned long ret_val = 0;
                size_t arg_len;
                if (edge_call_args_ptr(edge_call, &call_args, &arg_len) != 0) {
                    edge_call->return_data.call_status = CALL_STATUS_BAD_OFFSET;
                    return;
                }
                uintptr_t data_section = edge_call_data_ptr();
                int size = 0;
                switch (edge_call->call_id) {
                    case OCALL_GET_INPUT:
                        memcpy((void*)data_section, in, 16*sizeof(uint8_t));
                        size = 16*sizeof(uint8_t);
                        break;
                    case OCALL_SET_OUTPUT1:
                        memcpy(out1, (void*)call_args, 16);
                        ret_val = 0;
                        memcpy((void*)data_section, &ret_val, sizeof(unsigned long));
                        size = sizeof(unsigned long);
                        break;
                    case OCALL_SET_OUTPUT2:
                        memcpy(out2, (void*)call_args, 16);
                        ret_val = 0;
                        memcpy((void*)data_section, &ret_val, sizeof(unsigned long));
                        size = sizeof(unsigned long);
                        break;
                }
                if (edge_call_setup_ret(
                        edge_call, (void*)data_section, size)) {
                    edge_call->return_data.call_status = CALL_STATUS_BAD_PTR;
                } else {
                    edge_call->return_data.call_status = CALL_STATUS_OK;
                }
            }
            return;
        });

        edge_call_init_internals(
            (uintptr_t)enclave.getSharedBuffer(), enclave.getSharedBufferSize());

        enclave.run();

        return 0;
#else
        return test_aes(in, out1, out2);
#endif
    }

    static int dircall_aes(uint8_t *in,uint8_t *out1,uint8_t *out2) 
    {
        return test_aes(in, out1, out2);
    }
};

#endif // DO_AES_H
