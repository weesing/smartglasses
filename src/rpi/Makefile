CC=gcc
CFLAGS=-c -std=c99 -O3 -I ./
LDFLAGS=
SOURCES=st7735s_compat.c st7735s.c rpi_gpio.c rpi_spi.c fonts.c gfx.c
OBJECTS=$(patsubst %.c, %.o, $(wildcard *.c))
$(info $(OBJECTS))
HEADERS = $(wildcard *.h)
$(info $(HEADERS))
TARGET=lcd

all: $(TARGET)
    


%.o:%.c $(HEADERS)
	@echo "Object CC $@"
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
