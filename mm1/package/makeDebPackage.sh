#!/bin/sh

#Prepare files for installation (data)

mkdir -p ./debian/usr/sbin
cp ../Release/jobMan ./debian/usr/sbin
mkdir -p ./debian/usr/bin
cp ../resource/jobMan-pkexec ./debian/usr/bin

#Prepare controll file (control)

mkdir -p debian/DEBIAN
cp control ./debian/DEBIAN

dpkg-deb --build debian

mv debian.deb jobMan_1.1-1_amd64.deb

#Finally, cleanup
#rm -rf ./debian
