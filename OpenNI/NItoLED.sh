#!/bin/bash

mkdir real_time

for ((i = 0; i <= 100; i += 1))
do
./FrameToPgm frames/Frame_"$i".txt `printf "real_time/sample_%03d.pgm" $i`
sleep 0.02
done
