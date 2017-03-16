CC=clang++ #Compiler
CFLAGS= -c -std=c++11   -fPIE   #Compiler Flags
DEFINES=-DENABLE_DELETE
INCPATH=

LDFLAGS= -lfann#Linker options

SOURCES= main.cpp  #cpp files

OBJECTS=$(SOURCES:.cpp=.o)  #Object files
EXECUTEABLE=Main #Output name
all: $(HEADERS) $(SOURCES) $(EXECUTEABLE)

$(EXECUTEABLE): $(OBJECTS)
	$(CC)    $(OBJECTS) -o $(EXECUTEABLE) $(LDFLAGS)


.cpp.o:
	$(CC)  $(CFLAGS) $(INCPATH) $(DEFINES)   $< -o $@


clean:  ; rm *.o $(EXECUTEABLE) $(MOCFILES) $(HEADERS)
