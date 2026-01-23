# 🌐 Back-End in C

## 🗂️ Overview

Back-end built from scratch in C, using MySQL, Redis, libmicrohttpd, and cJSON within its stack.

It was initially created as an exercise to better understand the different components of an HTTP server,
however, the project is designed to be extensible if someone wishes to use it as a base.

Disclaimer: No AI or LLM was used in the development of this project.

## 🔥 Technology

The server runs on libmicrohttpd, a native C server focused on bringing modern functionality to the UNIX environment.

MySQL is used as the primary database.

Redis is also used to keep data in RAM, thus optimizing the use of limited resources.

Finally, cJSON is used to parse and format data into JSON.

## ✅ Requirements

To compile and run the program, you need a UNIX environment with the necessary libraries.

#### 🐧 Arch Linux

For Arch Linux, dependencies can be installed using the following command:

```
pacman -S gcc make mariadb libmicrohttpd redis hiredis cjson openssl
```

*Note*: You must run the command with _sudo_ or as _root_.

#### 🪟 Windows

The fastest way to run the program on Windows is through the Windows Subsystem for Linux. Arch Linux for WSL can be installed using the following commands:

First, update (or install) WSL:
```
wsl --update
```

Second, install Arch Linux inside WSL:
```
wsl --install archlinux
```

Done! Now follow the steps in the previous section to install the libraries used.

Tutorial: https://wiki.archlinux.org/title/Install_Arch_Linux_on_WSL

## 🐍 Execution

Once the necessary requirements are met, to compile the project you must run the following command from the repository root:

```
make
```

Then, for the program to function correctly, the necessary services must be started (such as root or via sudo)
so that the backend can perform CRUD operations.

```
systemctl start mariadb
systemctl start valkey
```

There is a dump of the database used for the development of this project, you can install it easily using the command:

```
mariadb BackEnd_in_C < db.sql
```

Finally, to run the program, run the following command (as _root_ or using _sudo_):

```
./bin/out
```
