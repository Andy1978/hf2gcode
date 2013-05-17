#!/bin/bash

echo "removing unwanted linebreak in $1..."
awk --re-interval 'BEGIN{FIELDWIDTHS = "5 3 1 1 5000"} $1~/[ 0-9]{5}/{printf("\n")} {printf("%s",$0)}' $1 > ../fixed/$1
