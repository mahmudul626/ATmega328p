# Variables - Ekhane change korle baki shob auto change hobe
FILENAME   = and_gate
PORT       = /dev/ttyACM0
DEVICE     = atmega328p
BAUD       = 115200
PROGRAMMER = arduino

# Default target
default: compile hex upload clean

compile:
	avr-gcc -Os -mmcu=$(DEVICE) -c $(FILENAME).c -o $(FILENAME).o
	avr-gcc -mmcu=$(DEVICE) $(FILENAME).o -o $(FILENAME).elf

hex:
	avr-objcopy -O ihex -R .eeprom $(FILENAME).elf $(FILENAME).hex

upload:
	avrdude -F -V -c $(PROGRAMMER) -p ATMEGA328P -P $(PORT) -b $(BAUD) -U flash:w:$(FILENAME).hex:i

clean:
	rm $(FILENAME).o $(FILENAME).elf $(FILENAME).hex
