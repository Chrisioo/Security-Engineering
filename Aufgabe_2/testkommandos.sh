#!/bin/bash

wget -O fussball-tabelle.html https://de.wikipedia.org/wiki/Fu%C3%9Fball-Bundesliga_2023/24
sed -e "s:<th\>:<td\>:g" -e "s:</th>:</td>:g" <fussball-tabelle.html > fussball-tabelle2.html
diff -u fussball-tabelle.html fussball-tabelle2.html > diff.txt
egrep -n "mw-.*Abschlusstabelle" fussball-tabelle.html > egrep.txt
tail -n +$(egrep -n "mw-.*Abschlusstabelle" fussball-tabelle.html | cut -d: -f1) fussball-tabelle.html > tabelle.html
fgrep "a href=" fussball-tabelle.html > links