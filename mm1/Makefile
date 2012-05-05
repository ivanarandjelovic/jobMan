VPATH = src

CFLAGS = `pkg-config --cflags glib-2.0 glibmm-2.4 giomm-2.4 gtkmm-3.0 gconfmm-2.6`
LFLAGS = `pkg-config --libs glib-2.0 glibmm-2.4 giomm-2.4 gtkmm-3.0 gconfmm-2.6`

all : jobMan

objects = job.o jobManWindow.o main.o services.o

jobMan : $(objects)
	g++ -o $@ $(objects) $(LFLAGS)
		
#job.o : src/job.cpp src/job.h
 
job.o : src/job.cpp src/job.h
	g++ $(CFLAGS) -c src/job.cpp 

jobManWindow.o : src/jobManWindow.cpp src/jobManWindow.h src/services.h
	g++ $(CFLAGS) -c src/jobManWindow.cpp

services.o : src/services.cpp src/services.h src/job.h
	g++ $(CFLAGS) -c src/services.cpp

main.o : src/main.cpp src/services.h src/jobManWindow.h
	g++ $(CFLAGS) -c src/main.cpp

.PHONY : clean
clean : 
	rm -v -f jobMan $(objects)

