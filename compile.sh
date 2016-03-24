#!/bin/bash

cd client
make $1; cp client_exec ../client_exec
cd ../server
make $1; cp server_exec ../server_exec
