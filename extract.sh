#!/bin/bash

for file in ./dataset-2/*.obj
do
  base=$(basename "$file" .obj)
  ./example.exe "$file" ./output/ 1024 8
done