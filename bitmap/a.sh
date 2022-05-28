#!/bin/bash
for loop in 10 20 30 40 50 60 70 80 90
do
    ./build/blend images/a.bmp $loop% images/b.bmp images/$loop.bmp
done
