#!/bin/sh

for file in $(find ./ -name "*.[h|c]")
do
    echo [FORMAT] $file
    indent  -bad -bap -bl -blf -bli0 -bbo -i4 -l79 -lp -ppi0 -psl -c33 -cd33 -cdb -nce -cli4 -cp33  -fc1 -fca -npcs -nprs -saf -sai -saw -sc -ts4 -nsob -nss $file
done