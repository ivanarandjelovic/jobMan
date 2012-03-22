#!/bin/sh

#Prepare files for installation (data)

mkdir -p ./debian/usr/sbin
cp ../Release/jobMan ./debian/usr/sbin

mkdir -p ./debian/usr/bin
cp ../resource/jobMan-pkexec ./debian/usr/bin

mkdir -p ./debian/usr/share/polkit-1/actions
cp ../resource/com.ubuntu.pkexec.jobMan.policy ./debian/usr/share/polkit-1/actions

mkdir -p ./debian/usr/share/applications
cp ../resource/jobMan.desktop ./debian/usr/share/applications

#Prepare controll file (control)

mkdir -p debian/DEBIAN
cp control ./debian/DEBIAN

dpkg-deb --build debian

mv debian.deb jobMan_1.1-1_amd64.deb

#Finally, cleanup
rm -rf ./debian