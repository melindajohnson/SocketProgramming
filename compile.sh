#!/bin/sh
g++ Server.cpp -o server -lpthread
g++ Client.cpp -o client

chmod 700 server
chmod 700 client


