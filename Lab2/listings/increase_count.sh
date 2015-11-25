#!/bin/bash
DIR=count_happens_here
NAME=1
CUR_SIZE=0
NEW_SIZE=0
WAVE=1
mkdir $DIR \
&& {
while (( $WAVE < 10 )) 
do
    touch $DIR/$NAME;
    let NEW_SIZE=$(ls -dgG $DIR | awk '{ print $3 }' )
    (( $CUR_SIZE < $NEW_SIZE )) && { CUR_SIZE=$(echo $NEW_SIZE); echo $CUR_SIZE" "$NAME; let WAVE+=1; };
    let NAME+=1;
done;
rm -rf $DIR;
exit 0; } \
|| { exit 1; }

