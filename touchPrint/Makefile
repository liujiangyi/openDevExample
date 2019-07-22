SREC_CAT=srec_cat.exe
BINCP=bin_merge.exe
BINENC=@bin_encrypt.exe
STLINK_CLI=cmd /c 'C:\\Program Files (x86)\\STMicroelectronics\\STM32 ST-LINK Utility\\ST-LINK Utility\\ST-LINK_CLI.exe'

DEVICE=STM32F103ze
JLINK=cmd /c 'D:\\Keil_v5\\ARM\\Segger\\JLink.exe -ExitOnError 1 -AutoConnect 1 -If swd -Device $(DEVICE) -Speed 4000'
GEN_INSTALL_JLINK=echo -e "r\nloadfile $^\nr\ng\nexit" 


FW_ENC=@Utils/YN622N-TRX-I_fw_encrypt.exe
DFU_ENC=@Utils/YN622N-TRX-I_dfu_encrypt.exe
YN_FW_GEN=ynfw_gen.py

PROD_ID:=0x000F
PROD_MODEL:="YN622N-II"
FW_FILE_VER:=0x02
HW_VERSION:=0x0100

all: YN622N-STM.hex app_enc.bin YN622N-STM.dfu makefile

bootloader.hex: Bootloader/build/Bootloader.hex
	cp $^ $@

app.hex: App/build/YN622N-II.hex
	cp $^ $@

dfu.hex: HidDfu/build/HidDfu.hex
	cp $^ $@

YN622N-STM.hex: bootloader.hex app.hex dfu.hex Makefile ../common.make
	$(SREC_CAT) bootloader.hex -intel app.hex -intel dfu.hex -intel -o $@ -intel
	$(SREC_CAT) -generate 0x800B7F4 0x800B7F6 -constant-l-e $(PROD_ID) 2 $@ -intel -exclude 0x800B7F4 0x800B7F6 -o $@ -intel
	$(SREC_CAT) -generate 0x800B7F8 0x800B7FA -constant-l-e $(FW_VERSION) 2 $@ -intel -exclude 0x800B7F8 0x800B7FA -o $@ -intel
	$(SREC_CAT) -generate 0x800B7FC 0x800B800 -constant-l-e 0x796E7466 4 $@ -intel -exclude 0x800B7FC 0x800B800 -o $@ -intel

app_enc.bin: app.hex
	$(SREC_CAT) $^ -intel -offset -0x8000000 -o $@ -binary
	$(FW_ENC) $@ $@

YN622N-STM.dfu: app_enc.bin  Makefile	../common.make
	$(YN_FW_GEN) --product-id $(PROD_ID) --product-model $(PROD_MODEL) --fw-ver $(FW_VERSION) --file-ver $(FW_FILE_VER)  --part $(FW_VERSION) $(HW_VERSION) HID app_enc.bin $@
	$(DFU_ENC) $@ $@

dist: YN622N-STM.dfu
	auto_handle.py dist YN622N-STM $(FW_VERSION) true

install: ./build/TOUCH.hex erase_mcu
	$(STLINK_CLI) -P $< -V -Rst
	$(STLINK_CLI) -Rst
	 # $(GEN_INSTALL_JLINK) > ./Jlink/install.jlink
	 # $(JLINK) ./Jlink/install.jlink

erase_mcu:
	# $(STLINK_CLI) -ME
	$(JLINK)  ./Jlink/erase.jlink

read_hex:
	$(STLINK_CLI) -Dump 0x0 0x20000 a.hex
read.bin:a.hex
	$(SREC_CAT) $< -intel -o $@ -binary

clean:
	-rm -rf *.bin  *.bak *.tmp test.hex test.dfu

.PHONY: all clean install dist install_test read_hex erase_mcu dfu_header.bin YN622N-TX-156.dfu
