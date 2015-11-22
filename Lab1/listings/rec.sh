#!/bin/bash 
append=">"
if [ "$1" == '-a' ]; then
    append=">>"
    output="$2"
else
    output="$1"
fi

[ "$output" == "" ] && output='record'

while [ true  ] 
do
read -rp "$ " command
eval printf \'\$ %s\\n\' \"$command\" $append $output
eval $command 2>&1 | tee -a $output
append=">>"
done

exit 0
