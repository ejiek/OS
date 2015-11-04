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
if [ "$append" == '>' ]; then
    echo '$ '$command > $output
else
   echo '$ '$command >> $output
fi
$command | tee -a $output

read -p "$ " command 
while [ "$command" != "exit" ] 
do 
echo '$ '$command >>  $output
$command | tee -a $output
read -p "$ " command 
done

exit 0
