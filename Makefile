all:
	gcc src/*.c src/model/*.c src/view/*.c src/controller/*.c -Wall -lmicrohttpd -lmariadb -lcurl -lcjson -lhiredis -lcrypto -O3 -o bin/out
