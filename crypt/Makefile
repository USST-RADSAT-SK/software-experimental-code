.PHONY: test clean

ENCRYPT = encrypt.c crypt_globals.c
DECRYPT = decrypt.c encrypt.c crypt_globals.c
FLAGS = -g aes.a -o

test:
	gcc test_encrypt.c $(DECRYPT) $(FLAGS) test_encrypt
	gcc test_decrypt.c $(DECRYPT) $(FLAGS) test_decrypt


clean:
	rm -f test_encrypt test_decrypt
