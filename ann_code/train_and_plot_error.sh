#!/bin/bash

make >/dev/null
echo "Starts Training"
echo "================"

./Main train > tmp.txt
sed -i -e '1,5d'  tmp.txt
gawk -i inplace '{ print $5 }' tmp.txt
sed -i -e 's/.$//g' tmp.txt

cat tmp.txt

rm tmp.txt
