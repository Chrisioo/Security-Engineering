#!/bin/sh

wget -O fussball-tabelle.html https://de.wikipedia.org/wiki/Fu%C3%9Fball-Bundesliga_2023/24                             # downloading https://de.wikipedia.org/wiki/Fussball-Bundesliga_2023/24 
sed -e "s:<th\>:<td\>:g" -e "s:</th>:</td>:g" <fussball-tabelle.html > fussball-tabelle2.html                           # <fussball-tabelle.html> is converted to <fussball-tabelle2.html>, all </th> are replaced by </td>
diff -u fussball-tabelle.html fussball-tabelle2.html > diff.txt                                                         # <fussball-tabelle.html> and <fussball-tabelle2.html> are compared, differences written in diff.txt
egrep -n "mw-.*Abschlusstabelle" fussball-tabelle.html > egrep.txt                                                      # lines that start with "mw-.*Abschlusstabelle" are written in egrep.txt
tail -n +$(egrep -n "mw-.*Abschlusstabelle" fussball-tabelle.html | cut -d: -f1) fussball-tabelle.html > tabelle.html   # last lines that start with "mw-.*Abschlusstabelle" are written in tabelle.html
fgrep "a href=" fussball-tabelle.html > links                                                                           # all lines that start with "a href=" are written in links