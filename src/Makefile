####################################################################################################
#                                     Make file LargeInt 	                                   #
####################################################################################################
INC:=-I../inc
CFLAGS:=-c -Wall $(INC) --std=c++14 -DD_DEBUG
SOURCES:= socket.cpp \
		communication.cpp \
		protocol.cpp \
		main.cpp

OBJDIR:=../obj
BINDIR:=../bin
LIBDIR:=../lib
LDFLAGS:=-L$(LIBDIR) 
OBJECTS:=$(SOURCES:.cpp=.o)
CC:=g++
EXECUTABLE:=rfbTest
LIBRARIES:= -pthread

all: cln MAKEDIR $(SOURCES) $(EXECUTABLE) 

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(addprefix $(OBJDIR)/, $(OBJECTS)) -o $(BINDIR)/$@ $(LIBRARIES)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $(OBJDIR)/$@

cln:
	rm -rf $(BINDIR)/$(EXECUTABLE)
	
clean:
	rm -rf $(BINDIR)/$(EXECUTABLE) $(addprefix $(OBJDIR)/, $(OBJECTS)) $(BINDIR) $(OBJDIR)

MAKEDIR:
	mkdir -p $(OBJDIR)
	mkdir -p $(BINDIR)
	mkdir -p $(LIBDIR)


