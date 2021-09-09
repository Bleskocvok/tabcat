#!/usr/bin/env bash


if [ "$#" -lt 2 ];
then
    {
        echo "Wrong arguments"
        echo "Usage: $0 [EXECUTABLE] [REVISION]"
    } 1>&2
    exit 1
fi


EXECUTABLE="$1"
APP_NAME=`basename $1`
REVISION="$2"
INSTALL_LOCATION="usr/local/bin"
MANPAGE="$3"


if ! [ -f control ];
then
    {
        echo "File 'control' not found"
    } 1>&2
    exit 1
fi


function extract()
{
    grep "$1:" control \
            | cut -d ':' -f 2 \
            | sed --posix 's/^[ ]*\([^ ]*\)[ ]*/\1/'
}


ARCHITECTURE=`extract Architecture`
VERSION=`extract Version`
FOLDER="${APP_NAME}_${VERSION}-${REVISION}_${ARCHITECTURE}"

# create the DEBIAN folder, required by dpkg
mkdir -p        "deb/$FOLDER/DEBIAN"
cp -f "control" "deb/$FOLDER/DEBIAN"
ln -s           "deb/$FOLDER/DEBIAN" "debian"

# create the installation folder
mkdir -p            "deb/$FOLDER/$INSTALL_LOCATION"
cp -f "$EXECUTABLE" "deb/$FOLDER/$INSTALL_LOCATION"

# creatge the manpage folder
if [ -n "$MANPAGE" ];
then
    mkdir -p        "deb/$FOLDER/usr/local/man/man1"
    cp "$MANPAGE"   "deb/$FOLDER/usr/local/man/man1/${APP_NAME}.1"
    gzip -f         "deb/$FOLDER/usr/local/man/man1/${APP_NAME}.1"
fi


DEPS=$(dpkg-shlibdeps -O "deb/$FOLDER/$INSTALL_LOCATION/$APP_NAME" \
        | cut -d '=' -f 2-)
CONTROL="deb/$FOLDER/DEBIAN/control"
CONTENT=`cat $CONTROL`
echo "$CONTENT" | sed --posix "s/^Depends:/Depends: ${DEPS}\n/g" > $CONTROL


echo "App name:     $APP_NAME"
echo "Architecture: $ARCHITECTURE"
echo "Version:      $VERSION"
echo "Depends:      $DEPS"
if [ -n "$MANPAGE" ];
then
echo "Manpage file: $MANPAGE"
fi

dpkg-deb --build --root-owner-group "deb/$FOLDER"


#
# cleanup
#
rm "debian"

