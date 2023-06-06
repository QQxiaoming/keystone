#include <syscall.h>

#include "eapp_utils.h"
#include "edge_call.h"
#include "string.h"
#include "AES/aes.h"

extern int test_aes(uint8_t* in, uint8_t* out1, uint8_t* out2);

int test_aes(uint8_t* in, uint8_t* out1, uint8_t* out2) {
    /* 256 bit key */
    uint8_t key[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                    0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
                    0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
                    0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f};

    uint8_t* w;  // expanded ke
    w = aes_init(sizeof(key));
    aes_key_expansion(key, w);
    aes_cipher(in /* in */, out1 /* out */, w /* expanded key */);
    aes_inv_cipher(out1, out2, w);
    free(w);

    return 0;
}

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

