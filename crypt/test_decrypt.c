#include "encrypt.h"
#include "decrypt.h"

#define SUCCESS 0
#define FAILURE 1


int testDecrypt(){
	uint8_t* msg = "The quick brown fox jumps over the lazy dog";
	int msgLen = strlen(msg);
	printf("Message: %s\n", msg);
	int newMsgLen = getPaddedBufSize(msgLen);
	uint8_t paddedMsg[newMsgLen];
	padBuffer(msg, paddedMsg, msgLen, newMsgLen);
	printf("paddedMsg: %s\n", paddedMsg);
	encrypt(paddedMsg, newMsgLen);
	printf("encrypted buffer: %s\n", paddedMsg);
    decrypt(paddedMsg, newMsgLen);
    printf("decrypted buffer: %s\n", paddedMsg);

}

void main(){
    printf("Testing decrypt:\n\n");

    testDecrypt();
}
