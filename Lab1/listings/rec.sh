#!/bin/bash 
append=">"
if [ "$1" == '-a' ]; then
    append=">>"
    output="$2"
else
    output="$1"
fi

if [ "$output" == "" ]; then 
    output='record'
fi

while [ true  ] 
do
read -p "$ " command
eval "echo '$ $command' $append $output"
eval $command | tee -a $output
append=">>"
done

exit 0
