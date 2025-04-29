TARGET  = IPC_client
SRC_DIR = ./
INC_DIR = ./
BUILD_DIR = ./build/

SIGN_DIR = ./SIGN
myPRIVKEY = privatekey_AP.der

CC = arm-linux-gnueabihf-gcc
CFLAGS += -I$(INC_DIR)
SRC     = $(wildcard $(SRC_DIR)*.c)
OBJ     = $(SRC:$(SRC_DIR)%.c=$(BUILD_DIR)%.o) #$(patsubst %.c, %.o, $(SRC))
DEP     = $(SRC:$(SRC_DIR)%.c=$(BUILD_DIR)%.d) #$(patsubst %.o, %.d, $(OBJ))

#LIB_DIR = ./lib/
#LIBS = $(wildcard $(LIB_DIR)*.a)

CFLAGS += -IPOSAPI/include
SRC +=   $(wildcard ./POSAPI/*.c)

CFLAGS += -IEMVK
SRC += 	$(wildcard ./EMVK/*.c)

CFLAGS += -IPEDK/include
SRC += 	$(wildcard ./PEDK/*.c)

CFLAGS += -IPEDS_hnb/include
SRC += 	$(wildcard ./PEDS_hnb/*.c)

# CFLAGS += -IGzip/APP
# CFLAGS += -IGzip/BSP_Include
# SRC +=   $(wildcard ./Gzip/APP/*.c)

CFLAGS += -Ictls/LV1/include
SRC  +=  $(wildcard ./ctls/LV1/*.c)

CFLAGS += -Ictls/LV2/include
SRC  +=  $(wildcard ./ctls/LV2/*.c)

# CFLAGS += -ISRED/include
# CFLAGS += -ISRED/ucl
# CFLAGS += -ISRED
# SRC += $(wildcard ./SRED/*.c)

CFLAGS += -IAPP
CFLAGS += -IAPP/include
SRC +=   $(wildcard ./APP/*.c)

CFLAGS += -IINC/BSP_Include
CFLAGS += -IINC/openssl
CFLAGS += -IINC

#CFLAGS +=-L./lib -lcrypto -lssl
LINKFLAG = -L./lib -lssl -lcrypto
#LINKFLAG=  -Bstatic -L./lib -lssl -lcrypto

CFLAGS += -D PCI_AP
CFLAGS += -D _PLATFORM_AS350
CFLAGS += -D _FLASH_32MB_
CFLAGS += -D _SCREEN_SIZE_240x320 
CFLAGS += -D _SCREEN_SIZE_320x240
# CFLAGS += -D _MOUNT_PRINTER_ 
CFLAGS += -D_GNU_SOURCE #for gatt.c ,definition O_DIRECT 
CFLAGS += -fstack-protector-all

.PHONY: clean
DLINKFLAG=-Bstatic -L/usr/arm-linux-gnueabihf/lib -lssl -lcrypto -lrt\
-Bdynamic -L/usr/arm-linux-gnueabihf/lib -pthread -ldl
$(TARGET) : $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@  $(DLINKFLAG) 

# --- Generate DIGEST.bin (from TARGET file) ---
	openssl dgst -sha256 -binary -out DIGEST.bin $(TARGET)

# --- Generate DIGEST_sig.bin (sign DIGEST.bin by using customer's private key) ---
	openssl rsautl -sign -in DIGEST.bin -keyform der -inkey $(SIGN_DIR)/$(myPRIVKEY) -out DIGEST_sig.bin

# --- Generate new TARGET with digital signature (original TARGET + DIGEST_sig.bin) ---
	$(SIGN_DIR)/CatDigest $(TARGET) -fa

${BUILD_DIR}%.o : %.c
	@echo $(dir $@)
	@if [ ! -d "$(dir $@)" ]; then mkdir -p $(dir $@); fi
	$(CC) --sysroot=/usr/arm-linux-gnueabi/include $(CFLAGS) -MMD -c $< -o $@ $(LINKFLAG)  $(DLINKFLAG) 
# Include all .d files
-include $(DEP)

# Build target for every single object file.
# The potential dependency on header files is covered by calling `-include $(DEP)`.
%.o : %.c
	$(CC) $(CFLAGS) -MMD -c $< -o $@  $(DLINKFLAG) 

## clean
clean:
	rm -f $(TARGET) $(OBJ) $(DEP) lib$(STATIC_LIB_NAME).a
	rm -r $(BUILD_DIR)
all :
	$(TARGET)