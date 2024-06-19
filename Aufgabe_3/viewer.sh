#!/bin/sh

# Funktion zur Anzeige von Bilddateien
view_image() {
    /usr/bin/xdg-open "$1"
}

# Funktion zur Anzeige von PDF-Dateien
view_pdf() {
    /usr/bin/evince "$1"
}

# Funktion zur Anzeige von Textdateien
view_text() {
    /usr/bin/less "$1"
}

# Funktion zur Anzeige von OpenDocument Texten
view_odt() {
    /usr/bin/libreoffice --writer "$1"
}

# Hauptfunktion zur Dateitypunterscheidung und Anzeige
main() {
    file_type=$(file "$1")

    if echo "$file_type" | grep -q "PDF document"; then
        view_pdf "$1"
    elif echo "$file_type" | grep -q "image"; then
        view_image "$1"
    elif echo "$file_type" | grep -q "text"; then
        view_text "$1"
    elif echo "$file_type" | grep -q "OpenDocument"; then
        view_odt "$1"
    else
        echo "Unbekannte Dateiendung: $file_type"
    fi
}

# Aufruf der Hauptfunktion mit dem übergebenen Dateinamen
main "$1"
