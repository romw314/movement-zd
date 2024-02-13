REQUIRE?=
PARAMS?=
TARGET?=movement-zd
STD:=c++20
GCC=g++
CC=g++
LINK_REQUIRE?=
LINK_PARAMS?=
BIN?=/usr/bin
LOCAL_BIN?=/usr/local/bin
BIN_NAME?=movement-zd
INSTALL_EXEC=$(LOCAL_BIN)/$(BIN_NAME)
INSTALLS=$(INSTALL_EXEC)

ADDITIONAL_OPTIONS:=$(shell cat additional_cc_options | tr "\n" " ")
ADDITIONAL_LINK_OPTIONS:=$(shell cat additional_link_options | tr "\n" " ")
LIBS:=$(patsubst %,-l%,$(shell cat libs))
PARAMS+= $(ADDITIONAL_OPTIONS)
LINK_PARAMS+= $(ADDITIONAL_LINK_OPTIONS)

# $(wildcard *.cpp /xxx/xxx/*.cpp): get all .cpp files from the current directory and dir "/xxx/xxx/"
SRCS=$(wildcard *.cpp)
# $(patsubst %.cpp,%.o,$(SRCS)): substitute all ".cpp" file name strings to ".o" file name strings
OBJS=$(patsubst %.cpp,%.o,$(SRCS))

all: $(TARGET)
real winstall: all
	sudo $(MAKE) install
nolink: $(OBJS)

%.o: %.cpp
	$(CC) $(PARAMS) $(REQUIRE) -c -g -o $@ -std=$(STD) $<

$(TARGET): $(OBJS)
	$(CC) $(LINK_PARAMS) $(LINK_REQUIRE) -o $@ $^ $(LIBS)

clean:
	rm -rf $(TARGET) *.o

install: $(BIN)/$(BIN_NAME) $(TARGET)
	chown root:root $<
	chmod ug+s $<
uninstall:
	rm -rf $(INSTALLS)

$(INSTALL_EXEC): $(TARGET) 
	cp $< $@

.PHONY: all real nolink install uninstall clean
