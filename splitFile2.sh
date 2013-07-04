#!/bin/bash
awk  '{print $1 "|" $4 "|" $7 "|" $8 "|" $9 "|" $10 "|" $11 "|" $14}' access.log | awk ' NR%1000000==1 {i++} {print >> "file"i; close("file"i)}'

