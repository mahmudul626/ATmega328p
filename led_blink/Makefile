MCU = atmega328p
CC = avr-gcc
CFLAGS = -Os -mmcu=$(MCU)

PORT = /dev/ttyACM0
PROGRAMMER = arduino
BAUD = 115200

TARGET = main

SRC = \
	main.c \
	led.c

OBJ = $(SRC:.c=.o)

all: build upload clean

build:
	$(CC) $(CFLAGS) -c $(SRC)

	$(CC) $(CFLAGS) $(OBJ) -o $(TARGET).elf

	avr-objcopy -O ihex -R .eeprom \
	$(TARGET).elf \
	$(TARGET).hex

upload:
	avrdude \
	-F \
	-V \
	-c $(PROGRAMMER) \
	-p m328p \
	-P $(PORT) \
	-b $(BAUD) \
	-U flash:w:$(TARGET).hex:i

clean:
	rm -f *.o *.elf *.hex
