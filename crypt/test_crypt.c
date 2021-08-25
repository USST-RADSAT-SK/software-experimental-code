// File to test encrpytion and decrpyion functions, an the various helper funcituons associated. There are various commented out print statements within the file which may be helpful
// Author: Thomas Slotoom ths943@usask.ca



#include "RAes.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SUCCESS 0
#define FAILURE 1

#define ON 1
#define OFF 0

#define ivSize 16
/* A 128 bit IV */
uint8_t iv[ivSize];

/* A global variable for storing whether to print debug info or not*/
int printFlag = OFF;


void randomizeIv();
int assertIntEqual(int expected, int result);
int testGetPaddedBufSize();
int testPadBuffer();
int testPadBufferHelper(uint8_t* buf, int bufSize);
int testEncryptAndDecrypt();
int testEncryptHelper();
void printBuffer(uint8_t* message, uint8_t* buff, int buffSize);


void randomizeIv(){
	for (int i=0; i<ivSize; i++){
		iv[i] = (uint8_t)rand();
	};
	// printf("IV: %s\n", iv);
}


int testGetPaddedBufSize(){
	int result;
	int input;
	int expected;
	int test_result = SUCCESS;

	input = 0;
	expected = 16;
	result = getPaddedBufSize(input);
	test_result |= assertIntEqual(expected, result);

	input = 14;
	expected = 16;
	result = getPaddedBufSize(input);
	test_result |= assertIntEqual(expected, result);

	input = 16;
	expected = 32;
	result = getPaddedBufSize(input);
	test_result |= assertIntEqual(expected, result);

	input = 36;
	expected = 48;
	result = getPaddedBufSize(input);
	test_result |= assertIntEqual(expected, result);

	input = 64;
	expected = 80;
	result = getPaddedBufSize(input);
	test_result |= assertIntEqual(expected, result);

	input = 180;
	expected = 192;
	result = getPaddedBufSize(input);
	test_result |= assertIntEqual(expected, result);
	return test_result;
}

int assertIntEqual(int expected, int result){
	if (expected != result){
		printf("ERROR: expected %d but got %d\n", expected, result);
		return FAILURE;
	}
	return SUCCESS;
}

int testPadBuffer(){
	int testResult = SUCCESS;
	testResult |= testPadBufferHelper("asdfasdfasdf", 12);
	testResult |= testPadBufferHelper("asdfasdfasdfasdf", 16);
	testResult |= testPadBufferHelper("T", 1);
	testResult |= testPadBufferHelper("asdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdf"
			"asdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdf"
			"asdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdf"
			"asdfasdfasdfasdfasdfasdfasdfasdfasdf", 192);
	testResult |= testPadBufferHelper("", 0);
	return testResult;
}

int testPadBufferHelper(uint8_t* buf, int bufSize){

	int newBufSize = getPaddedBufSize(bufSize);

	uint8_t newBuf[newBufSize];
	padBuffer(buf, newBuf, bufSize, newBufSize);
	// the last digit of pkcs7 padded buffers should match the amount of bytes added to the buffer
	uint8_t paddingByte =  AES_DECRYPT_MODULUS_SIZE - (bufSize % AES_DECRYPT_MODULUS_SIZE);
	if ((uint8_t)newBuf[newBufSize - 1] == paddingByte) { 
		return SUCCESS; 
	}
	else {
		printBuffer("Error with buffer: ", buf, bufSize);
		return FAILURE;
	}
}


void printBuffer(uint8_t* message, uint8_t* buf, int bufSize){
	printf("%s ", message);
	for (int i=0; i<bufSize; i++){
		printf("%c", buf[i]);
	}
 	printf("\n\n");
}

int testEncryptAndDecrypt(){
	int testResult = SUCCESS;
	testResult |= testEncryptHelper("The quick brown fox jumps over the lazy dog");
	testResult |= testEncryptHelper("");
	testResult |= testEncryptHelper("1");
	// 300 bytes
	testResult |= testEncryptHelper("012345678901234567890123456789"
	"012345678901234567890123456789"
	"012345678901234567890123456789"
	"012345678901234567890123456789"
	"012345678901234567890123456789"
	"012345678901234567890123456789"
	"012345678901234567890123456789"
	"012345678901234567890123456789"
	"012345678901234567890123456789"
	"012345678901234567890123456789");
	// 15 bytes
	testResult |= testEncryptHelper("0123456789ABCDE");
	// 16 bytes
	testResult |= testEncryptHelper("0123456789ABCDEF");
	// 17 bytes
	testResult |= testEncryptHelper("0123456789ABCDEF0");

	uint8_t * msg;
	for (int msgSize=0; msgSize<=257; msgSize++){
		msg = (uint8_t*) malloc(msgSize * sizeof(uint8_t));
		for (int msgIndex=0; msgIndex<msgSize; msgIndex++){
			msg[msgIndex] = 65 + (msgIndex % 26); // get some message containing various letters of the alphabet
		}
		testResult |= testEncryptHelper(msg);
		free(msg);
	}
	return testResult;
}

int testEncryptHelper( uint8_t* msg ) {
// Returns the memcmp of the original message and the encrypred / decrypted message.
	randomizeIv();

	int msgLen = strlen(msg);

	uint8_t * msgCopy;
	msgCopy = (uint8_t*) malloc(msgLen * sizeof(uint8_t));
	memcpy(msgCopy, msg, msgLen);

	int newMsgLen = getPaddedBufSize(msgLen);


	uint8_t paddedMsg[newMsgLen];
	padBuffer(msg, paddedMsg, msgLen, newMsgLen);

	aesEncrypt(paddedMsg, newMsgLen, iv);

	size_t actualDataLength = aesDecrypt(paddedMsg, newMsgLen, iv);
	uint8_t decryptedMessage[actualDataLength];

	int memCmp = memcmp(paddedMsg, msgCopy, msgLen);

	if (memCmp != 0){
		printBuffer("Error with message: ", paddedMsg, msgLen);
		printBuffer("Should have been: ", msgCopy,  msgLen);
	}
	free(msgCopy);
	return memCmp;
}

int testEncryptWriteToFile(){
	// This test encrypts a buffer, places that buffer into a file, and then pulls the text from
	// that file and decrypts this. The idea is to simluate the distance that will be evident 
	// when sending messages from the ground station to the satellite. 
	randomizeIv();
	uint8_t* message = "sphinx of black quartz judge my vow"; // contains all vowels, and also some random crap
	int msgLen = strlen(message);
	int newMsgLen = getPaddedBufSize(msgLen);
	uint8_t* newMessage = (uint8_t*)malloc(newMsgLen * sizeof(uint8_t));
	padBuffer(message, newMessage, msgLen, newMsgLen);
	printBuffer("Regular message: ", newMessage, msgLen);

	aesEncrypt(newMessage, newMsgLen, iv);
	FILE *fPtr = fopen("encryptedText.txt", "w");
	for (int i=0; i<newMsgLen; i++){
		fprintf(fPtr, "%c", newMessage[i]);
	}
	fclose(fPtr);

	// nuke the original buffer
	memset(newMessage, 0, newMsgLen);
	puts(newMessage);
	free(newMessage);	

	FILE *fPtr2 = fopen("encryptedText.txt", "r");
	uint8_t* textFromFile = (uint8_t*)malloc(newMsgLen * sizeof(uint8_t));
	for (int i=0; i<newMsgLen; i++){
		textFromFile[i] = fgetc(fPtr);
	}
	printBuffer("Message taken from file: ", textFromFile, newMsgLen);
	size_t actualMessageSize = aesDecrypt(textFromFile, newMsgLen, iv);
	printBuffer("Regular message:", message, msgLen);
	printBuffer("Decrypted message taken from file:", textFromFile, actualMessageSize);

	fclose(fPtr2);
	int memCmp = memcmp(message, textFromFile, msgLen);
	free(textFromFile);
	if (memCmp == 0) { 
		return SUCCESS;
	}
}


void main(int argc, char** argv){
	srand(time(NULL)); // seed rand() using the current time so that rand() will give different results every time.
	printf("----- Testing getPaddedBufSize: -----\n\n");
	if (testGetPaddedBufSize() == SUCCESS){
		printf("testGetPaddedBufSize() succeeded.\n\n");
	}
	else{
		printf("testGetPaddedBufSize() failed.\n\n");
	}

	printf("----- Testing Pad Buffer: -----\n\n");
	if (testPadBuffer() == SUCCESS) {
		printf("testPadBuffer() succeeded.\n\n");
	}
	else{
		printf("testPadBuffer() failed.\n\n");
	}

	printf("----- Testing encrypt and decrypt: -----\n\n");
	if (testEncryptAndDecrypt() == SUCCESS) {
		printf("testEncryptAndDecrypt() succeeded.\n\n");
	}
	else{
		printf("testEncryptAndDecrypt() failed.\n\n");
	}

	printf("----- Testing aesEncrypt by writing to and reading from file: -----\n\n");
	if (testEncryptWriteToFile() == SUCCESS){
			printf("testEncryptWriteToFile() succeeded.\n\n");
	}
	else{
		printf("testEncryptWriteToFile() failed.\n\n");
	}

}
