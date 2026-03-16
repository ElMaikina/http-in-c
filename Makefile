# C files from where code is compiled and run
CFILES = src/*.c src/models/*.c src/repositories/*.c src/controllers/*.c src/utils/*.c

# Libraries and dependencies for the project
CLIBS = -lmicrohttpd -lmariadb -lcurl -lcjson -lhiredis -lcrypto

# Compilation flags for GCC
CFLAGS = -Wall -O3

# Where the appliaction binary is compiled
COUT = bin/out

all:
	gcc $(CFILES) $(CLIBS) -o $(COUT)