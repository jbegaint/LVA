#! bin/sh

mkdir real_time

for ((i = 0; i <= 100; i += 1))
do
./FrameToPgm frames/Frame_"$i".txt real_time/sample_"$i".pgm
sleep 0.02
done
