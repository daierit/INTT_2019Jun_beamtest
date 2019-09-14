#!/bin/bash

path="/sphenix/data/data02/sphenix/t1439/2019/prdf/macro/*.root"
files=$(ls $path)
for filename in $files
do
echo ${filename##*/} | cut -c11-13 >> Run_list.txt
done
