# Quick build/flash commands

BOARD ?= nrf52840dk_nrf52840

.PHONY: build flash clean menuconfig

build:
	west build -b $(BOARD) --pristine always

flash:
	west flash --softreset

clean:
	rm -rf build

menuconfig:
	west build -t menuconfig
