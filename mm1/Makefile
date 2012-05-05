#Search path for source files
VPATH = src

#Compiler flags
CFLAGS = `pkg-config --cflags glib-2.0 glibmm-2.4 giomm-2.4 gtkmm-3.0 gconfmm-2.6`
#Linker flags
LFLAGS = `pkg-config --libs glib-2.0 glibmm-2.4 giomm-2.4 gtkmm-3.0 gconfmm-2.6`

#Main target
.PHONY: all
all : jobMan

#List all object files here
objects = job.o jobManWindow.o main.o services.o

#Main target depends on all object files
jobMan : $(objects)
	g++ -o $@ $(objects) $(LFLAGS)
		
#General prerequisites
%.cpp : %.h

#Non standard prerequisites/inlcudes:

main.o jobManWindow.o : services.h
main.o : jobManWindow.h
services.o : job.h

#General rule to build (compile) object files
%.o : %.cpp %.h
	g++ $(CFLAGS) -c $<

#Simple cleanup of object files and executable file
.PHONY : clean
clean : 
	rm -v -f jobMan $(objects)

