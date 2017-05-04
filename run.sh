#!/bin/bash

if [[ $(which pio) == "" ]]; then
	echo "[-] PlatformIO not found, installing"
	echo "$ brew install platformio"
	brew install platformio
fi

clear && \
pio run -t upload && \
clear && \
pio serialports monitor -p $(pio serialports list | grep usbmodem)
