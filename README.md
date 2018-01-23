# INTRODUCTION

*Submitted by Kshitij Gupta(20161234) and Pratik Jain(20161050)*

This assignment uses Linux Sockets for sending and receiving data
over a network. It demonstrates basic usage and implementation
of **Sockets**.

## Basic server to send and receive files (Non-Persistent Connection)

The ```server``` and ```client``` will run on ```PORT 5998```.
To change the PORT number you need to change the ```PORT``` Macro
in both ```server.c``` and ```client.c```.

### Server

```
./server
```

### Client

```
./client <file_name>
```


## Persistent Connection

### Server

```
./server
```

### Client

```
./client <file_name_1> <file_name_2> ..
```


**Comparison** :
Tested using 10 files that have 1MB data each.
 - Persistent connection takes 0.34 seconds to transfer all the files.
 - Non-persistent connection takes 0.45 seconds.
Persistent is faster since the socket connections are established only once hence less time
and less over head of all the system calls makes it faster. It will be even faster if we
transfer large number of files.
