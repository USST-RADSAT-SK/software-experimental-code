#include "decrypt.h"


#define BUF_LEN_STEP_SIZE 16



int decrypt(uint8_t* ciphertext, int ciphertextlen);
void depadBuffer(uint8_t* decryptedText, int decryptedtextlen);

/**
 * @brief Encrypts a buffer in with an AES CBC cipher
 * @header "comms-processing/crypt/encrypt.h"
 * @param plaintext - plaintext value to be decrypt
 * @param plaintextLen - The length of the plaintext value
 * @return Null
 */
int decrypt(uint8_t* ciphertext, int ciphertextlen){
    struct AES_ctx ctx;
    AES_init_ctx_iv(&ctx, key, iv);
    AES_CBC_decrypt_buffer(&ctx, ciphertext, ciphertextlen);
    depadBuffer(ciphertext, ciphertextlen);
}

void depadBuffer(uint8_t* decryptedText, int decryptedtextlen){
    int padLength = decryptedText[decryptedtextlen - 1];
    printf("paddingLength: %d\n", padLength);
    decryptedText[decryptedtextlen - padLength] = '\0';
}
