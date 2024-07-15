#!/bin/bash

# User hash entries
declare -A USERS
USERS["Steffi.Jones"]='$1$O7v0C21Z$2FH7ib2Dxtoq6B83qTgON1'
USERS["Marco.Reus"]='$1$Jebn3vQ5$2k..iqxtXNwfsCFAamWCS0'
USERS["Almuth.Schult"]='$1$0ngrMRa1$uXLzWhnrYzmiRM3fi8Nde1'
USERS["Manuel.Neuer"]='$1$1aaPttrp$VoF2rkOyC/tE.DxzQuuIY1'
USERS["Birgit.Prinz"]='$1$7ieEwjFr$T/jwatbzqhLZNVDEfymB41'

# Array to store found passwords
declare -A FOUND_PASSWORDS

while read -r word; do
    for username in "${!USERS[@]}"; do
        hash="${USERS[$username]}"
        salt=$(echo $hash | cut -d'$' -f3)
        target_hash=$(echo $hash | cut -d'$' -f4)

        # Generate hash for the current word
        generated_hash=$(openssl passwd -1 -salt "$salt" "$word")
        echo "Generated hash: $generated_hash for $word"
        # Compare the generated hash with the target hash
        if [[ "$generated_hash" == *"$target_hash"* ]]; then
            echo "Password for $username found: $word"
            FOUND_PASSWORDS["$username"]="$word"
            unset USERS["$username"]
        fi

        # If all passwords are found, exit
        if [ ${#USERS[@]} -eq 0 ]; then
            break 2
        fi
    done
done < "words.txt"

# Output all usernames with their found passwords
for username in "${!FOUND_PASSWORDS[@]}"; do
    echo "Password for $username: ${FOUND_PASSWORDS[$username]}"
done

# Output usernames for which passwords were not found
for username in "${!USERS[@]}"; do
    echo "Password for $username not found."
done

exit 0