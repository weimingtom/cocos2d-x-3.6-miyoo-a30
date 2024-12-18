#!/bin/bash

DEPENDS=' '
DEPENDS+=' libx11-dev'
DEPENDS+=' libxmu-dev'
DEPENDS+=' libglu1-mesa-dev'
DEPENDS+=' libgl2ps-dev'
DEPENDS+=' libxi-dev'
DEPENDS+=' libglfw3-dev'
DEPENDS+=' cmake'
DEPENDS+=' g++'
DEPENDS+=' gcc'
#DEPENDS+=' libzip-dev'
DEPENDS+=' libcurl4-gnutls-dev'
DEPENDS+=' libfontconfig1-dev'
#DEPENDS+=' libsqlite3-dev'
DEPENDS+=' libglew-dev'
DEPENDS+=' libopenal-dev'
DEPENDS+=' libalut-dev'
DEPENDS+=' libxrandr-dev libwebp-dev libtiff5-dev libjpeg-dev libpng12-dev libfreetype6-dev libvorbis-dev libwebsockets-dev '
#libvorbisfile3

#ubuntu 16.04 32bit

MISSING=
echo "Checking for missing packages ..."
for i in $DEPENDS; do
    if ! dpkg-query -W --showformat='${Status}\n' $i | grep "install ok installed" > /dev/null; then
        MISSING+="$i "
    fi
done

if [ -n "$MISSING" ]; then
    TXTCOLOR_DEFAULT="\033[0;m"
    TXTCOLOR_GREEN="\033[0;32m"
    echo -e $TXTCOLOR_GREEN"Missing packages: $MISSING.\nYou may be asked for your password for package installation."$TXTCOLOR_DEFAULT
    sudo apt-get --force-yes --yes install $MISSING
fi

