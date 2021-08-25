.PHONY: test clean

CRYPT = RAes.c -Itiny-aes tiny-aes/pkcs7Padding.c tiny-aes/aes.c

test:
	gcc test_crypt.c $(CRYPT) -o test_crypt


clean:
	rm -f test_crypt encryptedText.txt
