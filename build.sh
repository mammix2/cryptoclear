#!/bin/bash

set -e

date
ps axjf

#################################################################
# Update Ubuntu and install prerequisites for running Cryptoclear   #
#################################################################
sudo apt-get update
#################################################################
# Build Cryptoclear from source                                     #
#################################################################
NPROC=$(nproc)
echo "nproc: $NPROC"
#################################################################
# Install all necessary packages for building Cryptoclear           #
#################################################################
sudo apt-get install -y qt4-qmake libqt4-dev libminiupnpc-dev libdb++-dev libdb-dev libcrypto++-dev libqrencode-dev libboost-all-dev build-essential libboost-system-dev libboost-filesystem-dev libboost-program-options-dev libboost-thread-dev libboost-filesystem-dev libboost-program-options-dev libboost-thread-dev libssl-dev libdb++-dev libssl-dev ufw git
sudo add-apt-repository -y ppa:bitcoin/bitcoin
sudo apt-get update
sudo apt-get install -y libdb4.8-dev libdb4.8++-dev

cd /usr/local
file=/usr/local/cryptoclear
if [ ! -e "$file" ]
then
        sudo git clone https://github.com/CCLR/cryptoclear/.git
fi

cd /usr/local/cryptoclear/src
file=/usr/local/cryptoclear/src/cryptocleard
if [ ! -e "$file" ]
then
        sudo make -j$NPROC -f makefile.unix
fi

sudo cp /usr/local/cryptoclear/src/cryptocleard /usr/bin/cryptocleard

################################################################
# Configure to auto start at boot                                      #
################################################################
file=$HOME/.cryptoclear
if [ ! -e "$file" ]
then
        sudo mkdir $HOME/.cryptoclear
fi
printf '%s\n%s\n%s\n%s\n' 'daemon=1' 'server=1' 'rpcuser=u' 'rpcpassword=p' | sudo tee $HOME/.cryptoclear/cryptoclear.conf
file=/etc/init.d/cryptoclear
if [ ! -e "$file" ]
then
        printf '%s\n%s\n' '#!/bin/sh' 'sudo cryptocleard' | sudo tee /etc/init.d/cryptoclear
        sudo chmod +x /etc/init.d/cryptoclear
        sudo update-rc.d cryptoclear defaults
fi

/usr/bin/cryptocleard
echo "Cryptoclear has been setup successfully and is running..."
exit 0

