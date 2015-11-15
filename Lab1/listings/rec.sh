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
read -p "$ " command
eval "echo '$ $command' $append $output"
eval $command 2>&1 | tee -a $output
append=">>"
done

exit 0
