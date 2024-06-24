#!/bin/sh

for i in $*             # $* returns all arguments
do                      # loop through all arguments
    echo "Hallo $i"     # $i is the current argument, returns "Hallo $i"
done                    # loop is finished
exit 0                  # exit with code 0 if no errors occurred