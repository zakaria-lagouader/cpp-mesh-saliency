#!/bin/bash

for file in ./dataset-2/*.obj
do
  base=$(basename "$file" .obj)
  ./example.exe "$file" ./face_normals_and_area/
done