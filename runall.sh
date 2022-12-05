#!/bin/bash

for FILE in `ls Tests/ | grep [^~]$ | grep -v .py$` 
do
	echo "Start simulation with file $FILE"
	./src/build/IHK Tests/$FILE 
done
