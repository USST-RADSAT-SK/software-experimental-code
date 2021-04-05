.PHONY: test clean

ENCRYPT = encrypt.c crypt_globals.c pkcs7_padding.c
DECRYPT = decrypt.c encrypt.c crypt_globals.c pkcs7_padding.c
FLAGS = -g aes.a -o

test:
	gcc test_encrypt.c $(DECRYPT) $(FLAGS) test_encrypt


clean:
	rm -f test_encrypt
