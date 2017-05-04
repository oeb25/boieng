#!/bin/bash

if [[ $(which pio) == "" ]]; then
	echo "[-] PlatformIO not found, installing"
	echo "$ brew install platformio"
	brew install platformio
fi

pio run -t upload && \
pio serialports monitor -p $(pio serialports list | grep usbmodem)
