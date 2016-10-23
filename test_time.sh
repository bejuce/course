#!/bin/bash
for((size = 4000000; size <= 6000000; size += 500000))
do
    ./a.out $size
done
