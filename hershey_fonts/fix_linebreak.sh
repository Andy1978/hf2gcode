#!/bin/bash

echo "removing unwanted linebreak in $1..."
sed 's/\x1A//g' $1 | tr -d '\r' | awk --re-interval 'BEGIN{FIELDWIDTHS = "5 3 1 1 200"} $1~/[ 0-9]{5}/{if(NR>1) printf("\n")} {printf("%s",$0)}' > ../fixed/$1
