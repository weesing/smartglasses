import ubluetooth
from micropython import const
import machine
import time, math

ble = ubluetooth.BLE()
while not ble.active():
    ble.active(True)

ble.config(gap_name='weesing-esp32')

IRQ_CENTRAL_CONNECT = const(1)
IRQ_CENTRAL_DISCONNECT = const(2)
IRQ_GATTS_WRITE = const(3)
CHAR_BUFFER_SIZE = 1024
CHAR_HANDLE = 0x15

# Define UUIDs (or can use GATT service name in terms of bytes)
# UUID Generator: https://www.uuidgenerator.net/
PERIPHERAL_UUID = ubluetooth.UUID(0xfc7c)
# Indicate GATT characteristics
# List of characteristics: https://www.bluetooth.com/specifications/gatt/characteristics/
PERIPHERAL_CHAR = (ubluetooth.UUID(0x218d), ubluetooth.FLAG_READ |
                   ubluetooth.FLAG_WRITE | ubluetooth.FLAG_NOTIFY,)
PERIPHERAL_SERVICE = ((PERIPHERAL_UUID, (PERIPHERAL_CHAR,),),)

((dl,),) = ble.gatts_register_services(PERIPHERAL_SERVICE)
print('Extending characteristics buffer size to', str(CHAR_BUFFER_SIZE), 'bytes')
ble.gatts_write(CHAR_HANDLE, bytes(CHAR_BUFFER_SIZE))
print('Characteristics buffer size extended to', str(CHAR_BUFFER_SIZE), 'bytes')
ble.gatts_write(CHAR_HANDLE, 'Initialized'.encode())


def bt_irq(event, data):
    print('Event -', event)
    if event == IRQ_CENTRAL_CONNECT:
        conn_handle, addr_type, addr = data
        print('Central device CONNECTED')
        for i in range(len(addr)):
            print(hex(addr[i]), end=':')
        print('')
    elif event == IRQ_CENTRAL_DISCONNECT:
        conn_handle, addr_type, addr = data
        print('Central device DISCONNECTED')
        for i in range(len(addr)):
            print(hex(addr[i]), end=':')
        advertise()
        print('')
    elif event == IRQ_GATTS_WRITE:
        # Handle data here
        conn_handle, attr_handle = data
        value = ble.gatts_read(attr_handle)
        strValue = value.decode('utf-8')
        print('Value -', strValue)
        print('')


ble.irq(bt_irq)


def adv_encode(adv_type, value):
    return bytes((len(value) + 1, adv_type,)) + value


def adv_encode_name(name):
    return adv_encode(const(0x09), name.encode())


def advertise():
    # Indicate GATT service name in reverse byte order in second argument of adv_encode(0x03, b'')
    # List of services: https://www.bluetooth.com/specifications/gatt/services/
    ble.gap_advertise(100, adv_encode(0x01, b'\x06') + adv_encode(0x03, b'\x1c\x18') +
                      adv_encode(0x19, b'\xc1\x03') + adv_encode_name('weesing-esp32'))


advertise()


def testDisplay():
    from st7735s import TFT
    from sysfont import sysfont
    from machine import SPI, Pin
    from st7735s import TFTColor

    spi = SPI(1, baudrate=100000000, polarity=0, phase=0)
    # dc, rst, cs
    tft = TFT(spi, 2, 4, 15)
    tft.init_7735(tft.BLUETAB80x160)
    tft.fill(TFT.BLACK)
    # tft.text((0, 0), "The big brown fox jumps over the lazy dog. The big brown fox jumps over the lazy dog!",
    #          TFT.YELLOW, sysfont)
    tileWidth = 4
    tileHeight = 4
    from random import random
    for y in range(0, 80 - tileHeight, tileHeight):
      for x in range(0, 160 - tileWidth, tileWidth):
        r = int(random() * 0xff)
        g = int(random() * 0xff)
        b = int(random() * 0xff)
        print('Fill rect at ', (x, y), 'with color', hex(r), hex(g), hex(b))
        tft.fillrect((x, y), (tileWidth, tileHeight), TFTColor(r, g, b))



testDisplay()
