

## Serial Communication on MacOS or Linux:
In order to communicate with the device you need to use the following command.
```
    minicom -b 115200 -o  -D /dev/tty.usbmodem112101
```
## Upload .UF2 File to RP2040 on MacOS or Linux:
To upload the .UF2 file go inside your build folder and use the following command.
```
 cp -X blink.uf2 /Volumes/RPI-RP2/
```