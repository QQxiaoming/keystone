#include <syscall.h>

#include "eapp_utils.h"
#include "edge_call.h"
#include "string.h"

#include "../AES/test_func.h"


#define OCALL_GET_INPUT   1
#define OCALL_SET_OUTPUT1 2
#define OCALL_SET_OUTPUT2 3

__attribute__((section(".text._start"))) int main()  {
    uint8_t in[16];
    uint8_t out1[16], out2[16];
    unsigned long retval;

    ocall(OCALL_GET_INPUT, NULL, 0, in, 16*sizeof(uint8_t));

    test_aes(in, out1, out2);

    ocall(OCALL_SET_OUTPUT1, out1, 16, &retval, sizeof(uint8_t));
    ocall(OCALL_SET_OUTPUT2, out2, 16, &retval, sizeof(uint8_t));

    EAPP_RETURN(0);
}

