/**
 * @file RAes.c
 * @date April 26 2021
 * @author Thomas Slotboom (ths943)
 */

#include "RAes.h"
#include "stdio.h"


/* A 128 bit key */
uint8_t key[] = { 0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6,
     0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c };





/***************************************************************************************************
                                          GLOBAL VARIABLES
***************************************************************************************************/

/** A 128 bit key, in the future we will pull this from fram */
static uint8_t privateKey[] = { 0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6,
     0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c };


/***************************************************************************************************
                                             PUBLIC API
***************************************************************************************************/




/**
 * @brief Encrypts a buffer in with an AES CBC cipher
 * @header "comms-processing/crypt/encrypt.h"
 * @param plaintext - plaintext value to be encrypted. Must already be padded with padBuffer.
 * @param plaintextLen - The length of the plaintext value
 * @return Null
 */
void aesEncrypt(uint8_t* plaintext, uint32_t plaintextLen, uint8_t* initializationVector){
    struct AES_ctx ctx;
    AES_init_ctx_iv(&ctx, key, initializationVector);
    AES_CBC_encrypt_buffer(&ctx, plaintext, plaintextLen);
}

/**
 * @brief Decrypts a buffer in with an AES CBC cipher
 * @param cipherText plaintext value to be decrypted
 * @param cipherTextLength The length of the plaintext value
 * @return The actual size of the decrypted data
 */
uint32_t aesDecrypt(uint8_t* cipherText, uint32_t cipherTextLength, uint8_t* initializationVector) {
	// sets up the context for decryption and initiailizes the initialization vector
	struct AES_ctx context;
	// at some point we will have to pull the key from memory
	AES_init_ctx_iv(&context, privateKey, initializationVector);

	AES_CBC_decrypt_buffer(&context, cipherText, cipherTextLength);
	uint32_t actualDataLength = (uint32_t)pkcs7_padding_data_length(cipherText, cipherTextLength, AES_DECRYPT_MODULUS_SIZE);
	return actualDataLength;
}


/**
 * @brief Pads a buffer to be a multiple of 16 bytes
 * @header "comms-processing/crypt/encrypt.h"
 * @param buff - The incoming buffer that will be padded
 * @param paddedBuff - The outgoing buffer that will contain the padded data
 * @param buffLen - The length of the incoming buffer
 * @param newBuffLen - The length that the outgoing buffer will be   
 * @pre Need to know the size of the old and new buffers before calling the function
 * @return Null
 */
void padBuffer(uint8_t* buff, uint8_t* paddedBuff, uint32_t buffLen, uint32_t newBuffLen){
	int paddingSize = newBuffLen - buffLen;
	memset(paddedBuff, 0, newBuffLen);
	for (int i=0; i<buffLen; i++){
		paddedBuff[i] = buff[i];
	}
	pkcs7_padding_pad_buffer(paddedBuff, buffLen, newBuffLen, AES_DECRYPT_MODULUS_SIZE);
}

/**
 * @brief Gets the next multiple of 16 from the size of the input buffer
 * @header "comms-processing/crypt/encrypt.h"
 * @param plaintextLen - The size of the unpadded buffer containing data to be encrypted.
 * @pre need to have a variable storing the size of the plaintext buffer
 * @return The size of the new buffer to be created
 */
uint32_t getPaddedBufSize(uint32_t plaintextLen){
    // Special case for when the buffer size is already a multiple of 16:
    if (plaintextLen % AES_DECRYPT_MODULUS_SIZE == 0){
	uint32_t newBufSize = plaintextLen + AES_DECRYPT_MODULUS_SIZE;
        return newBufSize; //If no padding is to be added, pad with 16 bytes
    }
    else{
    	uint32_t newBufSize = plaintextLen +
        (AES_DECRYPT_MODULUS_SIZE - (plaintextLen % AES_DECRYPT_MODULUS_SIZE));
    	return newBufSize;
    }
}
