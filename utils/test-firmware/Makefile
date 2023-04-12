OLOADER=openFPGALoader
ECPPROG=./ecpprog

CROSS=riscv64-unknown-elf-
CFLAGS=-g -Os -mabi=ilp32 -march=rv32i


all: dirs out/everest.bit

##
## Synthesis
##

out/everest.json: out/everest_fw.hex
out/everest.json: rtl/firmsoc.v rtl/picorv32.v rtl/simpleuart.v
	yosys -ql log/everest-syn.log -p 'synth_ecp5 -top firmsoc -json $@' rtl/firmsoc.v rtl/picorv32.v rtl/simpleuart.v
	cat log/everest-syn.log | sed -n '/statistics/,/CHECK/p'

out/everest.config: out/everest.json
	nextpnr-ecp5 -l log/everest-pnr.log --json $^ --lpf brd/everest.lpf --textcfg $@ --85k --speed 6 --freq 50 --package CSFBGA285
	cat log/everest-pnr.log | sed -n '/Device utilisation/,/Placed/p'

out/everest.bit: out/everest.config
	ecppack --svf-rowsize 100000 --svf out/everest.svf $< $@

##
## Simulation
##

out/everest_tb.vvp: sim/firmsoc_tb.v sim/clkdiv.v rtl/attosoc.v rtl/picorv32.v rtl/simpleuart.v
	mkdir -p log out
	iverilog -s testbench -o $@ $^

sim: out/everest_tb.vvp out/everest_fw.hex
	vvp -N $<


##
## Flash
##

upload: out/everest.bit
	$(OLOADER) -c ft2232 --ftdi-channel 0 --verbose-level 9 $<

flash: out/everest.bit
	$(ECPPROG) $<

##
## Firmware
##

out/everest_fw.elf: src/start.s src/firmware.c
	$(CROSS)gcc -mabi=ilp32e -march=rv32i -Wl,-Bstatic,-T,brd/everest_sections.lds,--strip-debug -ffreestanding -nostdlib -o $@ $^
	$(CROSS)size --format berkley out/everest_fw.elf

out/everest_fw.bin: out/everest_fw.elf
	$(CROSS)objcopy -O binary $^ /dev/stdout > $@

out/everest_fw.hex: out/everest_fw.bin
	python3 src/makehex.py $^ 4096 > $@


# ---- Clean ----

dirs:
	mkdir -p log out

clean:
	rm -rf out log
	rm -f testbench.vcd

.PHONY: upload flash sim
