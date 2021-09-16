CC=gcc
CFLAGS=-c -std=c99 -O3 -I ./
LDFLAGS=
SOURCES=main.c st7735.c gpio.c spi.c
OBJECTS=$(patsubst %.c, %.o, $(wildcard *.c))
HEADERS = $(wildcard *.h)
TARGET=lcd

all: $(TARGET)
    


%.o:%.c $(HEADERS)
	@echo "CC $@"
	@$(CC) $(CFLAGS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
	@echo "LD $@"
	@$(CC) $(OBJECTS) -Wall $(LIBS) -o $@


clean:
	@echo "[RM]	*.o"
	@-rm -f *.o
	@echo "[RM]	$(TARGET)"
	@-rm -f $(TARGET)

run:
	sudo ./lcd &
	sudo fbset -xres 160 -yres 120 -vxres 160 -vyres 120 -depth 16
	sudo pkill lcd
	sudo ./lcd &

