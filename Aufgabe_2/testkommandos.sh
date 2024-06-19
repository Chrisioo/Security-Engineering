#!/bin/bash

wget -O fussball-tabelle.html https://de.wikipedia.org/wiki/Fu%C3%9Fball-Bundesliga_2023/24                             # https://de.wikipedia.org/wiki/Fussball-Bundesliga_2023/24 wird heruntergeladen
sed -e "s:<th\>:<td\>:g" -e "s:</th>:</td>:g" <fussball-tabelle.html > fussball-tabelle2.html                           # <fussball-tabelle.html wird in <fussball-tabelle2.html umgewandelt, alle </th> werden durch </td> ersetzt
diff -u fussball-tabelle.html fussball-tabelle2.html > diff.txt                                                         # <fussball-tabelle.html und <fussball-tabelle2.html werden verglichen, wird in diff.txt geschrieben
egrep -n "mw-.*Abschlusstabelle" fussball-tabelle.html > egrep.txt                                                      # Die Zeile, in der die Bundesligatabelle beginnt, wird in egrep.txt geschrieben
tail -n +$(egrep -n "mw-.*Abschlusstabelle" fussball-tabelle.html | cut -d: -f1) fussball-tabelle.html > tabelle.html   # Die Zeilen, die mit "mw-.*Abschlusstabelle" beginnen, werden in tabelle.html geschrieben
fgrep "a href=" fussball-tabelle.html > links                                                                           # Alle Zeilen, die mit "a href=" beginnen, werden in links geschrieben