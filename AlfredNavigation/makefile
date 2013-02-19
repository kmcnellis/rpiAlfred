.PHONY: build

build:
	@echo "Uploading ./build/AlfredNavigation.hex..."
	"avrdude" "-C/home/santob2/Program/Arduino/arduino-1.0.3/hardware/tools/avrdude.conf" -q -q -V -patmega328p -carduino -P/dev/ttyUSB0 -b115200 -D "-Uflash:w:./build/AlfredNavigation.hex:i"
	
