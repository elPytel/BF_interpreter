#!/bin/bash
# By Pytel

apt_dependencies="dependencies.txt"

# Install apt dependencies
sudo apt-get update
if [ -f $apt_dependencies ]; then
    xargs sudo apt-get -y install < $apt_dependencies
fi

echo -e "Done!"