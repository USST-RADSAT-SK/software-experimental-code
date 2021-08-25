/**
 * @file RAes.h
 * @date April 26 2021
 * @author Thomas Slotboom (ths943)
 */

#ifndef RAES_H_
#define RAES_H_

#include <stdint.h>
#include <string.h>
#include <aes.h>
#include <pkcs7Padding.h>


/***************************************************************************************************
                                            DEFINITIONS
***************************************************************************************************/

/** Modulus size used for padding of encrypted buffers */
#define AES_DECRYPT_MODULUS_SIZE ((uint8_t)16)

uint32_t aesDecrypt(uint8_t* cipherText, uint32_t cipherTextLength, uint8_t* initializationVector);
uint32_t getPaddedBufSize(uint32_t plaintextLen);
void padBuffer(uint8_t* buff, uint8_t* paddedBuff, uint32_t buffLen, uint32_t newBuffLen);
void aesEncrypt(uint8_t* plaintext, uint32_t plaintextLen, uint8_t* initializationVector);


#endif /* RAES_H_ */
