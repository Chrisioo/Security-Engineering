#!/bin/sh

sed -n '/id="Abschlusstabelle"/,/<\/table>/p' fussball-tabelle.html |                    # Extract the HTML table with id="Abschlusstabelle" from fussball-tabelle.html
    grep -o 'href.*'  |                                                                  # Extract lines containing 'href'
    sed '1d' |                                                                           # Remove the first line
    cut -d'>' -f2- |                                                                     # Cut the line at the first '>', keeping the part after it
    sed 's/<\/a>//g' |                                                                   # Remove the closing '</a>' HTML tag
    egrep -n '*' |                                                                       # Add line numbers to each line
    sed 's/:/. /' |                                                                      # Replace ':' with '. ' to format the line numbers
    sed 's/([A-Za-z])//g' > tabelle.txt                                                  # Remove all letters, leaving only numbers and symbols, and save to tabelle.txt
