SREC_CAT=srec_cat.exe
STLINK_CLI=cmd /c 'C:\\Program Files (x86)\\STMicroelectronics\\STM32 ST-LINK Utility\\ST-LINK Utility\\ST-LINK_CLI.exe'
KEIL=cmd /c 'D:\\Keil_v5\\UV4\\UV4.exe'

DEVICE=STM32F103ZET6
JLINK=cmd /c 'D:\\Keil_v5\\ARM\\Segger\\JLink.exe -ExitOnError 1 -AutoConnect 1 -If swd -Device $(DEVICE) -Speed 4000'
GEN_INSTALL_JLINK=echo -e "r\nloadfile $^\nr\ng\nexit" 

FW_VERSION:=0x0103
MAJOR_VER=$(shell echo $(FW_VERSION) | cut -b 4)
MINOR_VER=$(shell printf "%02d" `echo $(FW_VERSION) | cut -b 1-2,5-6`)
FW_FILE_VER:=0x02
HW_VERSION:=0x0100

app.hex: build/myTouch.hex
	cp $^ $@

install: app.hex 
	# $(STLINK_CLI) -P $< -V -Rst
	# $(STLINK_CLI) -V -Rst
	$(GEN_INSTALL_JLINK) > ./Jlink/install.jlink
	$(JLINK) ./Jlink/install.jlink

erase_mcu:
	$(STLINK_CLI) -ME
#         $(JLINK)  ./Jlink/erase.jlink

read_hex:
	$(STLINK_CLI) -Dump 0x0 0x20000 a.hex

read.bin:a.hex
	$(SREC_CAT) $< -intel -o $@ -binary

clean:
	-rm -rf *.bin  *.bak *.tmp *.hex *.dfu


dist: YN968N-II.dfu
	auto_handle.py dist YN968N-II $(FW_VERSION) 

.PHONY: all clean install dist install_test read_hex erase_mcu 
