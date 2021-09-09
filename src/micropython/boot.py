# This is script that run when device boot up or wake from sleep.

import network
import time
from wifi_cfg import WIFI_SSID, WIFI_PASSWORD

wifi = network.WLAN(network.STA_IF)
while not wifi.active():
    wifi.active(True)

print('Connecting to WIFI')
wifi.connect(WIFI_SSID, WIFI_PASSWORD)
time.sleep_ms(5000)
while not wifi.isconnected():
    wifi.connect(WIFI_SSID, WIFI_PASSWORD)
    time.sleep_ms(5000)

print('WIFI connected')
