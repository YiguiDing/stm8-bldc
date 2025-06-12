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
# 搜素路径
vpath %.c $(sort $(dir $(C_SOURCES)))
######################################
#               Release              #
######################################
#目录
${OUT_DIR}/Release/:
	mkdir -p $@
#编译
${OUT_DIR}/Release/%.rel: %.c ${OUT_DIR}/Release/
	${CC} ${C_FLAGS} ${C_INCLUDES} ${C_DEFS} --compile-only $< -o $@
#链接
RELEASE_OBJECTS = $(addprefix ${OUT_DIR}/Release/,$(notdir $(C_SOURCES:.c=.rel)))
${OUT_DIR}/Release/${TARGET}.hex: ${RELEASE_OBJECTS}
	${CC} ${LD_FLAGS} --out-fmt-ihx ${RELEASE_OBJECTS} -o $@
#命名
release: ${OUT_DIR}/Release/${TARGET}.hex
######################################
#                Debug               #
######################################
DEBUG = --debug --out-fmt-elf
#目录
${OUT_DIR}/Debug/:
	mkdir -p $@
#编译
${OUT_DIR}/Debug/%.rel: %.c ${OUT_DIR}/Debug/
	${CC} ${C_FLAGS} ${DEBUG} ${C_INCLUDES} ${C_DEFS} --compile-only $< -o $@
#链接
DEBUG_OBJECTS = $(addprefix ${OUT_DIR}/Debug/,$(notdir $(C_SOURCES:.c=.rel)))
${OUT_DIR}/Debug/${TARGET}.elf: ${DEBUG_OBJECTS}
	${CC} ${LD_FLAGS} ${DEBUG} ${DEBUG_OBJECTS} -o $@
#命名
debug: ${OUT_DIR}/Debug/${TARGET}.elf
######################################
all: release debug
######################################
clean:
	rm -rf ${OUT_DIR}
######################################
program: all
	tools/stm8flash-cygwin64.exe -c stlinkv2 -p stm8s003f3  -w ${OUT_DIR}/Release/${TARGET}.hex
######################################
# todo: unknown error in flash command
_program: release
	${OPENOCD} \
	-f interface/stlink-dap.cfg \
	-f target/stm8s003.cfg \
	-c "init" \
	-c "reset halt" \
	-c "flash write_image erase ${OUT_DIR}/${TARGET}.hex 0x8000" \
	-c "reset" \
	-c "shutdown"
######################################
_debug:
	${OPENOCD} \ 
	-f interface/stlink-dap.cfg \
	-f target/stm8s003.cfg \
	-c "init" \
	-c "reset halt"
######################################