#include "encrypt.h"
#include "decrypt.h"

#define SUCCESS 0
#define FAILURE 1

int assertIntEqual(int expected, int result);
int testGetPaddedBufSize();
int testPadBuffer();
int testPadBufferHelper(uint8_t* buf, int bufSize);
int testEncrypt();
int testEncryptHelper();
int getLenAndPrintMsg(uint8_t* msg);
void printBuffer(uint8_t* message, uint8_t* buff, int buffSize);

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
}

int assertIntEqual(int expected, int result){
	if (expected != result){
		printf("ERROR: expected %d but got %d\n", expected, result);
		return FAILURE;
	}
	return SUCCESS;
}

int testPadBuffer(){
	testPadBufferHelper("asdfasdfasdf", 12);
	testPadBufferHelper("asdfasdfasdfasdf", 16);
	testPadBufferHelper("T", 1);
	testPadBufferHelper("asdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdf"
			"asdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdf"
			"asdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdf"
			"asdfasdfasdfasdfasdfasdfasdfasdfasdf", 192);
	testPadBufferHelper("", 0);
}

int testPadBufferHelper(uint8_t* buf, int bufSize){
	printf("Unpadded buffer: %s\n", buf);
	printf("Size of unpadded buffer: %d\n", bufSize);
	int newBufSize = getPaddedBufSize(bufSize);
	printf("Size of padded buffer: %d\n", newBufSize);
	uint8_t newBuf[newBufSize];
	padBuffer(buf, newBuf, bufSize, newBufSize);
}

int testEncrypt(){

	testEncryptHelper("The quick brown fox jumps over the lazy dog");
	testEncryptHelper("");
	testEncryptHelper("1");
	// 300 bytes
	testEncryptHelper("012345678901234567890123456789"
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
	testEncryptHelper("0123456789ABCDE");
	// 16 bytes
	testEncryptHelper("0123456789ABCDEF");
	// 17 bytes
	testEncryptHelper("0123456789ABCDEF0");
	printf("\n\n---------------------------------------------------------\n\n");

	printf("Testing messages of all sizes from 0 - 257 bytes:\n");
	int testResult = SUCCESS;
	uint8_t * msg;
	uint8_t * msgCopy;
	for (int msgSize=0; msgSize<=257; msgSize++){
		msg = (uint8_t*) malloc(msgSize * sizeof(uint8_t));
		msgCopy = (uint8_t*) malloc(msgSize * sizeof(uint8_t));
		for (int msgIndex=0; msgIndex<msgSize; msgIndex++){
			msg[msgIndex] = 65 + (msgIndex % 26); // get some message containing various letters of the alphabet
			msgCopy[msgIndex] = 65 + (msgIndex % 26);
		}
		testEncryptHelper(msg);
		int memCmp = memcmp(msg, msgCopy, msgSize);
		testResult |= memCmp;
		if (memCmp != 0){
			printBuffer("Error with message: ", msg, msgSize);
			printBuffer("Should have been: ", msgCopy,  msgSize);
		}
		free(msg);
		free(msgCopy);
	}
	if (testResult == FAILURE){
		printf("Error detected when testing all buffer sizes from 0-257 bytes. \n\n");
	}
	else{
		printf("Testing messages of all sizes from 0 - 257 bytes was a success! \n\n");
	}

}

int testEncryptHelper( uint8_t* msg ) {
	int msgLen = getLenAndPrintMsg(msg);
	int newMsgLen = getPaddedBufSize(msgLen);
	printf("Message length: %d\n", msgLen);
	printf("Padded message length: %d\n", newMsgLen);
	uint8_t paddedMsg[newMsgLen];
	padBuffer(msg, paddedMsg, msgLen, newMsgLen);
	// printBuffer("paddedMsg:", paddedMsg, newMsgLen);
	encrypt(paddedMsg, newMsgLen);
	printBuffer("encrypted buffer:", paddedMsg, newMsgLen);
	size_t actualDataLength = decrypt(paddedMsg, newMsgLen);
	uint8_t decryptedMessage[actualDataLength];
	printf("Decrypted message length: %ld\n", actualDataLength);
	printBuffer("decrypted buffer:", paddedMsg, actualDataLength);
	printf("\n");
}

int getLenAndPrintMsg(uint8_t* msg){
	int msgLen = strlen(msg);
	printf("Message: %s\n", msg);
	return msgLen;
}

void printBuffer(uint8_t* message, uint8_t* buff, int buffSize){
	printf("%s ", message);
	for (int i=0; i<buffSize; i++){
		printf("%c", buff[i]);
	}
 	printf("\n\n");
}

int testEncryptWriteToFile(){
	uint8_t* message = "sphinx of black quartz judge my vow";
	int msgLen = strlen(message);
	int newMsgLen = getPaddedBufSize(msgLen);
	uint8_t* newMessage = (uint8_t*)malloc(newMsgLen * sizeof(uint8_t));
	padBuffer(message, newMessage, msgLen, newMsgLen);
	printBuffer("Regular message: ", newMessage, newMsgLen);

	encrypt(newMessage, newMsgLen);
	FILE *fPtr = fopen("encryptedText.txt", "w");
	for (int i=0; i<newMsgLen; i++){
		fprintf(fPtr, "%c", newMessage[i]);
	}
	fclose(fPtr);

	// nuke the original buffer
	memset(newMessage, 0, newMsgLen);
	free(newMessage);	

	FILE *fPtr2 = fopen("encryptedText.txt", "r");
	uint8_t* textFromFile = (uint8_t*)malloc(newMsgLen * sizeof(uint8_t));
	for (int i=0; i<newMsgLen; i++){
		textFromFile[i] = fgetc(fPtr);
	}
	printBuffer("Message taken from file: ", textFromFile, newMsgLen);
	size_t actualMessageSize = decrypt(textFromFile, newMsgLen);
	printBuffer("Decrypted message taken from file: ", textFromFile, actualMessageSize);
	printf("memcmp: %d\n", memcmp(textFromFile, message, msgLen));
	fclose(fPtr2);
	free(textFromFile);
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
	printf("Testing encrpyt by writing to and reading from file: \n\n");
	testEncryptWriteToFile();

}
