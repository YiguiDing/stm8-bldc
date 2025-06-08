######################################
include lib/STM8S_StdPeriph_Driver/Makefile
include src/Makefile
######################################
# C defines
C_DEFS+=-D STM8S003
C_INCLUDES+= 
C_SOURCES+= 
OUT_DIR=output
TARGET=target
######################################
C_FLAGS= -lstm8 -mstm8 --opt-code-size --opt-code-speed --std=c11
LD_FLAGS= -lstm8 -mstm8 --opt-code-size --opt-code-speed --std=c11
######################################
SDCC_PATH=/cygdrive/d/Applications/SDCC4.5.2
HEX2BIN_PATH=/cygdrive/d/Applications/Hex2bin-2.5/bin/Release
OPENOCD_PATH=/cygdrive/d/Applications/openocd-v0.12.0-i686-w64-mingw32
PATH+=:${SDCC_PATH}/bin:${HEX2BIN_PATH}/bin:${OPENOCD_PATH}/bin
######################################
CC=${SDCC_PATH}/bin/sdcc
LD=${SDCC_PATH}/bin/sdld
HEX=${SDCC_PATH}/bin/packihx
BIN=${HEX2BIN_PATH}/hex2bin
OPENOCD=${OPENOCD_PATH}/bin/openocd
######################################
# 命令的使用案例：
# 编译
# *.c -> *.rel
# sdcc -c ./a.c -o a.rel
# sdcc -c ./b.c -o b.rel
# 链接
# *.rel -> *.ihx
# sdld -i out.ihx a.rel b.rel					# tips: Binary file start = 00000000
# or:
# sdcc a.rel b.rel --out-fmt-ihx -o out.ihx		# tips: Binary file start = 00008000
# 格式转换
# *.ihx -> *.hex|*.bin
# packihx out.ihx > out.hex
# hex2bin out.hex
######################################
#目录
${OUT_DIR}:
	mkdir $@
#编译
${OUT_DIR}/%.rel: %.c | ${OUT_DIR}
	${CC} ${C_FLAGS} ${C_INCLUDES} ${C_DEFS} -c $< -o $@
OBJECTS = $(addprefix ${OUT_DIR}/,$(notdir $(C_SOURCES:.c=.rel)))
vpath %.c $(sort $(dir $(C_SOURCES)))
#链接
${OUT_DIR}/${TARGET}.ihx: ${OBJECTS}
	${CC} ${LD_FLAGS} ${OBJECTS} --out-fmt-ihx -o $@
#格式转换
${OUT_DIR}/${TARGET}.hex: ${OUT_DIR}/${TARGET}.ihx
	${HEX} $< > $@
#格式转换
${OUT_DIR}/${TARGET}.bin: ${OUT_DIR}/${TARGET}.hex
	${BIN} $<
######################################
build: ${OUT_DIR}/${TARGET}.bin
######################################
flash: ${OUT_DIR}/${TARGET}.bin
	tools/stm8flash-cygwin64.exe -c stlinkv2 -p stm8s003f3  -w ${OUT_DIR}/${TARGET}.bin
######################################
# todo: unknown error in flash command
_flash: ${OUT_DIR}/${TARGET}.bin
	${OPENOCD} \
	-f interface/stlink-dap.cfg \
	-f target/stm8s003.cfg \
	-c "init" \
	-c "reset halt" \
	-c "flash write_image erase ${OUT_DIR}/${TARGET}.bin 0x8000" \
	-c "reset" \
	-c "shutdown"
######################################
debug:
	${OPENOCD} \ 
	-f interface/stlink-dap.cfg \
	-f target/stm8s003.cfg \
	-c "init" \
	-c "reset halt"
######################################
clean:
	rm -rf ${OUT_DIR}
######################################