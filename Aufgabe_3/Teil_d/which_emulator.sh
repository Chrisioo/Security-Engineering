#!/bin/sh                                    # Specify the shell interpreter to use

if [ "$#" -ne 1 ]; then                      # Check if the number of arguments passed to the script is not equal to 1
    echo "Usage: $0 <program name>"          # Print a usage message if the number of arguments is incorrect
    exit 1                                   # Exit the script with a status code of 1 (indicating an error)
fi                                           

program=$1                                   # Assign the first argument passed to the script to the variable 'program'

OLDIFS=$IFS                                  # Save the current value of IFS (Internal Field Separator)
IFS=':'                                      # Set IFS to ':' to split the PATH variable correctly
for dir in $PATH; do                         # Iterate over each directory in the PATH variable
    if [ -x "$dir/$program" ]; then          # Check if the program exists and is executable in the current directory
        echo "$dir/$program"                 # Print the full path of the program
        IFS=$OLDIFS                          # Restore the original IFS value
        exit 0                               # Exit the script with a status code of 0 (indicating success)
    fi                                       
done                                         

IFS=$OLDIFS                                  # Restore the original IFS value (done here in case the loop completes without finding the program)

echo "$program: Command not found"           # Print an error message if the program was not found in any directory in PATH
exit 1                                       # Exit the script with a status code of 1 (indicating an error)
