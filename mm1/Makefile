#$(warning makefile restart = $(MAKE_RESTARTS))

#Search path for source files
VPATH = src

#Compiler
CC = g++

#Compiler flags
CFLAGS = `pkg-config --cflags glib-2.0 glibmm-2.4 giomm-2.4 gtkmm-3.0 gconfmm-2.6`
#Linker flags
LFLAGS = `pkg-config --libs glib-2.0 glibmm-2.4 giomm-2.4 gtkmm-3.0 gconfmm-2.6`

#Main target
.PHONY: all
all : jobMan


#List all object files here
objects = job.o jobManWindow.o main.o services.o

#List source files
sources = main.cpp job.cpp services.cpp jobManWindow.cpp

#Main target depends on all object files
jobMan : $(objects)
	g++ -o $@ $(objects) $(LFLAGS)

#Generate prerequisites for each source file
%.d : %.cpp
	@set -e; rm -f $@; \
	$(CC) -MM $(CFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$
		
#Include generated prerequisites
include $(sources:.cpp=.d)

#General rule to build (compile) object files
$(objects) : %.o : %.cpp %.h
	g++ $(CFLAGS) -c $<

#Simple cleanup of object files and executable file
.PHONY : clean
clean : 
	rm -v -f jobMan $(objects) *.d

