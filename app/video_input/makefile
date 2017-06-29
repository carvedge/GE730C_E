CC              := arm-buildroot-linux-uclibcgnueabi-gcc
CXX             := arm-buildroot-linux-uclibcgnueabi-g++

SELF_INC        := -I./inc  
RTMPDIR = /opt/ge730c/arm_compile/rtmpdump-2.3/librtmp/install
INCSDIR1 = -I$(RTMPDIR)/include
                 
INC_PATH        :=  $(SELF_INC) $(INCSDIR1)

PRE_LIB	        := -L./lib  -L$(RTMPDIR)/lib -lrtmp   \
					-L/opt/ge730c/arm_compile/openssl-1.0.0s -lssl -lcrypto 
LIB_PATH        := $(PRE_LIB)

LIB             := -lxm_av -lasound -lmembroker -lmsgbroker -lsyncringbuffer -lpthread

MODULE_DEFINES  :=
CFLAGS	        := $(MODULE_DEFINES)  -Wall -O2 -o 

# program name
TARGET          := 1.mod 
SRCC	          := $(wildcard *.c)
COBJ	          := $(SRCC:.c=.o)
PRESRCC	        := $(wildcard ./src/*.c)
PRECOBJ	        := $(PRESRCC:.c=.o)
SRCXX	          := $(wildcard *.cpp)
CXXOBJ          := $(SRCXX:.cpp=.o)
OBJ	            := $(COBJ)  $(PRECOBJ) $(OBJ)

$(TARGET):$(OBJ)
	$(CC) $(INC_PATH) $(LIB_PATH) $(CFLAGS) $@ $(OBJ) $(LIB)

$(COBJ):%.o:%.c 
	$(CC) $(INC_PATH) $(CFLAGS) $@ -c $<

$(PRECOBJ):%.o:%.c 
	$(CC) $(INC_PATH) $(CFLAGS) $@ -c $<

$(CXXOBJ):%.o:%.cpp 
	$(CXX) $(INC_PATH) $(CFLAGS) $@ -c $<

.PHONY:clean
clean:
	rm -f $(TARGET) *~ ./src/*.o 
