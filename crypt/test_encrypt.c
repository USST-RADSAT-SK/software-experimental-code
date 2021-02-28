#include "encrypt.h"
#include "decrypt.h"

#define SUCCESS 0
#define FAILURE 1

int assertIntEqual(int expected, int result);
int testGetPaddedBufSize();
int testPadBuffer();
int testEncrypt();
int getLenAndPrintMsg(uint8_t* msg);

int assertIntEqual(int expected, int result){
	if (expected != result){
		printf("ERROR: expected %d but got %d\n", expected, result);
		return FAILURE;
	}
	return SUCCESS;
}

int testGetPaddedBufSize(){
	int result;
	int input;
	int expected;

	input = 0;
	expected = 0;
	result = getPaddedBufSize(input);
	assertIntEqual(expected, result);

	input = 14;
	expected = 16;
	result = getPaddedBufSize(input);
	assertIntEqual(expected, result);

	input = 16;
	expected = 16;
	result = getPaddedBufSize(input);
	assertIntEqual(expected, result);

	input = 36;
	expected = 48;
	result = getPaddedBufSize(input);
	assertIntEqual(expected, result);

	input = 64;
	expected = 64;
	result = getPaddedBufSize(input);
	assertIntEqual(expected, result);

	input = 180;
	expected = 192;
	result = getPaddedBufSize(input);
	assertIntEqual(expected, result);
}

int testPadBuffer(){
	int bufSize, newBufSize;

	uint8_t buf1[] = "asdfasdfasdf";

	// Subtract 1 to get rid of null byte
	bufSize = sizeof(buf1) / sizeof(buf1[0]) - 1;
	printf("Unpadded buffer: %s\n", buf1);
	printf("Size of unpadded buffer: %d\n", bufSize);
	newBufSize = getPaddedBufSize(bufSize);
	printf("Size of padded buffer: %d\n", newBufSize);

	uint8_t newBuf1[192];
	padBuffer(buf1, newBuf1, bufSize, newBufSize);
	printf("Padded buffer: %s\n", newBuf1);


	uint8_t buf2[] = "asdfasdfasdfasdf";
	bufSize = sizeof(buf2) / sizeof(buf2[0]) - 1;
	printf("Unpadded buffer: %s\n", buf2);
	printf("Size of unpadded buffer: %d\n", bufSize);
	newBufSize = getPaddedBufSize(bufSize);
	printf("Size of padded buffer: %d\n", newBufSize);

	uint8_t newBuf2[newBufSize];
	padBuffer(buf2, newBuf2, bufSize, newBufSize);
	printf("Padded buffer: %s\n", newBuf2);


	uint8_t buf3[] = "T";
	bufSize = sizeof(buf3) / sizeof(buf3[0]) - 1;
	printf("Unpadded buffer: %s\n", buf3);
	printf("Size of unpadded buffer: %d\n", bufSize);
	newBufSize = getPaddedBufSize(bufSize);
	printf("Size of padded buffer: %d\n", newBufSize);

	uint8_t newBuf3[newBufSize];
	padBuffer(buf3, newBuf3, bufSize, newBufSize);
	printf("Padded buffer: %s\n", newBuf3);


	// size = 192 bytes
	uint8_t buf4[] = "asdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdf"
	"asdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdf"
	"asdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdf"
	"asdfasdfasdfasdfasdfasdfasdfasdfasdf";

	bufSize = sizeof(buf4) / sizeof(buf4[0]) - 1;
	printf("Unpadded buffer: %s\n", buf4);
	printf("Size of unpadded buffer: %d\n", bufSize);
	newBufSize = getPaddedBufSize(bufSize);
	printf("Size of padded buffer: %d\n", newBufSize);

	uint8_t newBuf4[newBufSize];
	padBuffer(buf4, newBuf4, bufSize, newBufSize);
	printf("Padded buffer: %s\n", newBuf4);
}

int testEncrypt(){
	uint8_t* msg = "The quick brown fox jumps over the lazy dog";
	int msgLen = getLenAndPrintMsg(msg);
	int newMsgLen = getPaddedBufSize(msgLen);
	uint8_t paddedMsg[newMsgLen];
	padBuffer(msg, paddedMsg, msgLen, newMsgLen);
	printf("paddedMsg: %s\n", paddedMsg);
	encrypt(paddedMsg, newMsgLen);
	printf("encrypted buffer: %s\n", paddedMsg);
	decrypt(paddedMsg, newMsgLen);
	printf("decrypted buffer: %s\n", paddedMsg);

	uint8_t* msg1 = "";
	msgLen = getLenAndPrintMsg(msg1);
	newMsgLen = getPaddedBufSize(msgLen);
	uint8_t paddedMsg1[newMsgLen];
	padBuffer(msg1, paddedMsg1, msgLen, newMsgLen);
	printf("paddedMsg: %s\n", paddedMsg1);
	encrypt(paddedMsg1, newMsgLen);
	printf("encrypted buffer: %s\n", paddedMsg1);
	decrypt(paddedMsg1, newMsgLen);
	printf("decrypted buffer: %s\n", paddedMsg1);

	uint8_t* msg2 = "1";
	msgLen = getLenAndPrintMsg(msg2);
	newMsgLen = getPaddedBufSize(msgLen);
	uint8_t paddedMsg2[newMsgLen];
	padBuffer(msg2, paddedMsg2, msgLen, newMsgLen);
	printf("paddedMsg: %s\n", paddedMsg2);
	encrypt(paddedMsg2, newMsgLen);
	printf("encrypted buffer: %s\n", paddedMsg2);
	decrypt(paddedMsg2, newMsgLen);
	printf("decrypted buffer: %s\n", paddedMsg2);

	uint8_t* threeHundredByteMsg = "1234567891234567891234567891234567891234567891234567891234567891234567891234567891234567891234567891234567123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789123456789";
	msgLen = getLenAndPrintMsg(threeHundredByteMsg);
	newMsgLen = getPaddedBufSize(msgLen);
	uint8_t paddedThreeHundredByteMsg[newMsgLen];
	padBuffer(threeHundredByteMsg, paddedThreeHundredByteMsg, msgLen, newMsgLen);
	printf("paddedMsg: %s\n", paddedThreeHundredByteMsg);
	encrypt(paddedThreeHundredByteMsg, newMsgLen);
	printf("encrypted buffer: %s\n", paddedThreeHundredByteMsg);
	decrypt(paddedThreeHundredByteMsg, newMsgLen);
	printf("decrypted buffer: %s\n", paddedThreeHundredByteMsg);

	uint8_t* msg4 = "1234123412341234";
	msgLen = getLenAndPrintMsg(msg4);
	newMsgLen = getPaddedBufSize(msgLen);
	uint8_t paddedMsg4[newMsgLen];
	padBuffer(msg4, paddedMsg4, msgLen, newMsgLen);
	printf("paddedMsg: %s\n", paddedMsg4);
	encrypt(paddedMsg4, newMsgLen);
	printf("encrypted buffer: %s\n", paddedMsg4);
	decrypt(paddedMsg4, newMsgLen);
	printf("decrypted buffer: %s\n", paddedMsg4);

}

int getLenAndPrintMsg(uint8_t* msg){
	int msgLen = strlen(msg);
	printf("Message: %s\n", msg);
	return msgLen;
}

void main(){
	printf("Testing getPaddedBufSize:\n\n");
	if (testGetPaddedBufSize() == SUCCESS){
		printf("testGetPaddedBufSize() succeeded.\n\n");
	}
	else{
		printf("testGetPaddedBufSize() failed.\n\n");
	}

	printf("Testing Pad Buffer:\n\n");
	testPadBuffer();
	printf("\n");

	printf("Testing encrypt:\n\n");
	testEncrypt();


}
