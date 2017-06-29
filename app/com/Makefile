#
# Main Makefile for broadcast snapshot server project!
#  (C) Deng zhaoqi 2013.3.30
#

PROJDIR = .
INCSDIR = $(PROJDIR)/include
CC		=  arm-buildroot-linux-uclibcgnueabi-gcc
CPP	    =  arm-buildroot-linux-uclibcgnueabi-g++


CC_SOURCE_FILES  += 	$(wildcard *.c)
CC_SOURCE_FILES  += 	$(wildcard $(INCSDIR)/*.c)
CC_OBJECTS 		= 	$(patsubst %.c, %.o, $(CC_SOURCE_FILES))

CPP_SOURCE_FILES 	= 	$(wildcard *.cpp)
CPP_OBJECTS 		= 	$(patsubst %.cpp, %.o, $(CPP_SOURCE_FILES))

CPP_FLAGS = -I/opt/ge730c/arm_compile/build_boost/include/ 
CPP_LDFLAGS     =	-lpthread     \
								-L/opt/ge730c/arm_compile/build_boost/lib    \
    							-lboost_system \
    							-lboost_atomic \
    							-lboost_thread    \

com.mod: $(CPP_OBJECTS) $(CC_OBJECTS)
	$(CPP) $(CPP_FLAGS) $^  $(CPP_LDFLAGS) -o $@ 
	
%.o: %.c
	$(CC) -c $(CPP_FLAGS) -std=gnu99 -o $@ $^
%.o: %.cpp
	$(CPP) -c $(CPP_FLAGS) -o $@ $^



clean:
	rm -f *.o com.mod
