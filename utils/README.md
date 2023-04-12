
## USB

Two USB devices with 3 communication channels:
  * *FT2232* as high-speed with:
      - **JTAG** (port A) connected with FPGA
      - **UART** (port B) via /dev/ttyUSB1 to FPGA
  * *FT601* as super-speed x32 bit **FIFO** with the FPGA

```
$ lsusb
Bus 002 Device 002: ID 0403:601f Future Technology Devices International, Ltd FT601 32-bit FIFO IC
Bus 001 Device 012: ID 0403:6010 Future Technology Devices International, Ltd FT2232C/D/H Dual UART/FIFO IC

$ dmesg
[   50.046996] usb 1-6: new high-speed USB device number 7 using xhci_hcd
[   50.173128] usb 1-6: New USB device found, idVendor=0424, idProduct=2422, bcdDevice= 0.a0
[   50.173143] usb 1-6: New USB device strings: Mfr=0, Product=0, SerialNumber=0
[   50.175616] hub 1-6:1.0: USB hub found
[   50.175662] hub 1-6:1.0: 2 ports detected
[   50.462999] usb 1-6.1: new high-speed USB device number 8 using xhci_hcd
[   50.556126] usb 1-6.1: New USB device found, idVendor=0403, idProduct=6010, bcdDevice= 7.00
[   50.556141] usb 1-6.1: New USB device strings: Mfr=1, Product=2, SerialNumber=3
[   50.556148] usb 1-6.1: Product: USB <-> Serial Converter
[   50.556154] usb 1-6.1: Manufacturer: FTDI
[   50.556158] usb 1-6.1: SerialNumber: V1.4_20210810
[   50.642878] usbcore: registered new interface driver ftdi_sio
[   50.642901] usbserial: USB Serial support registered for FTDI USB Serial Device
[   50.642976] ftdi_sio 1-6.1:1.0: FTDI USB Serial Device converter detected
[   50.643013] usb 1-6.1: Detected FT2232H
[   50.643156] usb 1-6.1: FTDI USB Serial Device converter now attached to ttyUSB0
[   50.643199] ftdi_sio 1-6.1:1.1: FTDI USB Serial Device converter detected
[   50.643224] usb 1-6.1: Detected FT2232H
[   50.643328] usb 1-6.1: FTDI USB Serial Device converter now attached to ttyUSB1
```

## JTAG

```
$ openFPGALoader --scan-usb
found 9 USB device
Bus device vid:pid       probe type      manufacturer serial               product
001 008    0x0403:0x6010 FTDI2232        FTDI         V1.4_20210810        USB <-> Serial Converter

$ openFPGALoader -c ft2232 --ftdi-channel 0 --detect
Jtag frequency : requested 6.00MHz   -> real 6.00MHz
index 0:
    idcode 0x1113043
    manufacturer lattice
    family ECP5
    model  LFE5UM-85
    irlength 8
```

## Test firmware

```
$ openFPGALoader -c ft2232 --ftdi-channel 0 --verbose-level 9 everest.bit
Jtag frequency : requested 6.00MHz   -> real 6.00MHz
Raw IDCODE:
- 0 -> 0x41113043
- 1 -> 0xffffffff
- 2 -> 0xffffffff
- 3 -> 0xffffffff
- 4 -> 0xffffffff
found 1 devices
index 0:
    idcode 0x1113043
    manufacturer lattice
    family ECP5
    model  LFE5UM-85
    irlength 8
File type : bit
Open file: DONE
Parse file: DONE
bitstream header infos
Part: LFE5U-85F-6CSFBGA285
idcode: 41113043
IDCode : 41113043
displayReadReg
    Config Target Selection : 0
    Done Flag
    Std PreAmble
    No err
Enable configuration: DONE
SRAM erase: DONE
Loading: [==================================================] 100.00%
Done
userCode: 00000000
Disable configuration: DONE
displayReadReg
    Config Target Selection : 0
    Done Flag
    Std PreAmble
    No err
```

## SPI flash

SPI is accessible via JTAG with [ecpprog](https://github.com/gregdavill/ecpprog) by Greg Davill.

```
$ ./ecpprog everest.bit
init..
IDCODE: 0x41113043 (LFE5U-85)
ECP5 Status Register: 0x00200100
reset..
flash ID: 0xC8 0x40 0x16
file size: 1964814
erase 64kB sector at 0x000000..
erase 64kB sector at 0x010000..
erase 64kB sector at 0x020000..
erase 64kB sector at 0x030000..
erase 64kB sector at 0x040000..
erase 64kB sector at 0x050000..
erase 64kB sector at 0x060000..
erase 64kB sector at 0x070000..
erase 64kB sector at 0x080000..
erase 64kB sector at 0x090000..
erase 64kB sector at 0x0A0000..
erase 64kB sector at 0x0B0000..
erase 64kB sector at 0x0C0000..
erase 64kB sector at 0x0D0000..
erase 64kB sector at 0x0E0000..
erase 64kB sector at 0x0F0000..
erase 64kB sector at 0x100000..
erase 64kB sector at 0x110000..
erase 64kB sector at 0x120000..
erase 64kB sector at 0x130000..
erase 64kB sector at 0x140000..
erase 64kB sector at 0x150000..
erase 64kB sector at 0x160000..
erase 64kB sector at 0x170000..
erase 64kB sector at 0x180000..
erase 64kB sector at 0x190000..
erase 64kB sector at 0x1A0000..
erase 64kB sector at 0x1B0000..
erase 64kB sector at 0x1C0000..
erase 64kB sector at 0x1D0000..
programming..  1964814/1964814
verify..       1964814/1964814  VERIFY OK
Bye.
```
