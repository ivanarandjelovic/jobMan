#!/bin/sh

set -x verbose #echo on


#Build it first

cd ..
./buildJobMan.sh
cd package


#Prepare files for installation (data)

mkdir -p ./debian/usr/sbin
cp ../jobMan ./debian/usr/sbin

mkdir -p ./debian/usr/bin
cp ../resource/jobMan-pkexec ./debian/usr/bin

mkdir -p ./debian/usr/share/polkit-1/actions
cp ../resource/com.ubuntu.pkexec.jobMan.policy ./debian/usr/share/polkit-1/actions

mkdir -p ./debian/usr/share/applications
cp ../resource/jobMan.desktop ./debian/usr/share/applications



#Prepare controll file (control)
mkdir -p debian/DEBIAN
cp control ./debian/DEBIAN



#Don't forget the debian-binary with the version
echo "2.0" > ./debian/DEBIAN/debian-binary


#Build it now
dpkg-deb --build debian

#Rename to propper name
mv debian.deb jobMan_0.2-1_amd64.deb

#Finally, cleanup
rm -rf ./debian
