######################################
include lib/STM8S_StdPeriph_Driver/Makefile
include src/Makefile
######################################
# C defines
C_DEFS += -D STM8S003
C_INCLUDES += 
C_SOURCES += 
OUT_DIR = output
######################################
C_FLAGS = \
	-lstm8 \
	-mstm8 \
	--std=c11 \
	-D STM8S003 \
######################################
SDCC_PATH = D:/Applications/SDCC4.5.2
HEX2BIN_PATH = D:/Applications/Hex2bin-2.5/bin/Release
OPENOCD_PATH = D:/Applications/openocd-v0.12.0-i686-w64-mingw32
PATH += ${SDCC_PATH}/bin:${HEX2BIN_PATH}/bin:${OPENOCD_PATH}/bin
######################################
CC = ${SDCC_PATH}/bin/sdcc
LD = ${SDCC_PATH}/bin/sdld
HEX = ${SDCC_PATH}/bin/packihx
BIN = ${HEX2BIN_PATH}/hex2bin
OPENOCD = ${OPENOCD_PATH}/bin/openocd
######################################
# 命令的使用案例：
# 编译
# sdcc -c ./a.c -o a.rel
# sdcc -c ./b.c -o b.rel
# 链接
# sdld -i out.ihx a.rel b.rel
# 格式转换
# packihx out.ihx > out.hex
# hex2bin out.hex
######################################
#目录
${OUT_DIR}:
	mkdir $@
#编译
${OUT_DIR}/%.rel: %.c | ${OUT_DIR}
	${CC} ${C_FLAGS} ${C_INCLUDES} -c $< -o $@
OBJECTS = $(addprefix ${OUT_DIR}/,$(notdir $(C_SOURCES:.c=.rel)))
vpath %.c $(sort $(dir $(C_SOURCES)))
#链接
${OUT_DIR}/output.ihx: ${OBJECTS}
	${LD} -i $@ ${OBJECTS}
#格式转换
${OUT_DIR}/output.hex: ${OUT_DIR}/output.ihx
	${HEX} $< > $@
#格式转换
${OUT_DIR}/output.bin: ${OUT_DIR}/output.hex
	${BIN} $<
######################################
build: ${OUT_DIR}/output.bin
######################################
write: ${OUT_DIR}/output.bin
	${OPENOCD} 
	-f interface/stlink.cfg \
	-f target/stm8s003.cfg \
	-c "init" \
	-c "reset halt" \
	-c "flash write_image erase ${OUT_DIR}/output.bin 0x8000" \
	-c "reset run" \
	-c "shutdown"
######################################
debug:
	${OPENOCD} \ 
	-f interface/stlink.cfg \
	-f target/stm8s003.cfg \
	-c "init" \
	-c "reset halt"
######################################
clean:
	rm -rf ${OUT_DIR}
######################################