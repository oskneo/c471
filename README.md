CMPT 471 Assignment 3
Dual stack TCP file transfer client and dual stack TCP file transfer server

Introduction:

The client can create a TCP connection in IPv4 or IPv6 to request file from server.
The server can create TCP connections in IPv4 and IPv6 to send files to clients.

Usage examples:


./server.o 33455 33446 1460
./client.o ::1 33446 localhost 1.txt 1460