all:
	gcc src/*.c src/models/*.c src/repositories/*.c src/controllers/*.c src/utils/*.c -Wall -lmicrohttpd -lmariadb -lcurl -lcjson -lhiredis -lcrypto -O3 -o bin/out
