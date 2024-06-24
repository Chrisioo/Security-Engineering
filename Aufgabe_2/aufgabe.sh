#!/bin/sh

awk '/id="Abschlusstabelle"/{flag=1} flag; /<\/table>/{flag=0}' fussball-tabelle.html |     # awk searches file fussball-tabelle.html for 'id="Abschlusstabelle"' and 'table'
        grep -o 'href.*'  |                                                                 # grep searches for pattern 'href.*'
        sed '1d' |                                                                          # sed removes first line
        cut -d'>' -f2- |                                                                    # cut cuts off lines after the '>', returns the rest
        sed 's/<\/a>//g' |                                                                  # sed deletes the '</a>' and replaces it with nothing
        egrep -n '*' |                                                                      # egrep searches for pattern '*'
        sed 's/:/. /' |                                                                     # sed replaces ':' with '. '
        sed 's/([A-Za-z])//g' > tabelle.txt                                                 # sed removes all letters and replaces them with nothing