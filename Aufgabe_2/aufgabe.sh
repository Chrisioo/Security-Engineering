#!/bin/sh

sed -n '/<h3><span class="mw-headline" id="Abschlusstabelle">/,/</table>/p' fussball-tabelle.html | \
                                                           grep -F "a href=" | \
                                                           sed -E 's/<[^>]+>//g' | \
                                                           sed '1d' | \
                                                           awk '{print NR ". " $0}' > test.txt