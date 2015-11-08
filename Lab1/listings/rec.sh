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

read -p "$ " command 
while [ "$command" != "exit" ] 
do
eval "echo '$ $command' $append $output"
eval $command | tee -a $output
read -p "$ " command 
append=">>"
done

exit 0
