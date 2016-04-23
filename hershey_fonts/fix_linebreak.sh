#!/bin/bash
##
## 20.05.2013 Andreas Weber
## Removing "SUB" (0x1A), CR (Windows Carriage Return) and linebreaks in a glyph command.

echo "removing unwanted linebreak and CR in $1"
sed 's/\x1A//g' $1 | tr -d '\r' | gawk --re-interval 'BEGIN{FIELDWIDTHS = "5 3 1 1 200"} $1~/[ 0-9]{5}/{if(NR>1) printf("\n")} {printf("%s",$0)}' > $2
