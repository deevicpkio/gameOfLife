#!/usr/bin/bash

rm ./build/bin/app.out
clang++ ./src/main.cpp -o ./build/bin/app.out -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

./run.sh
