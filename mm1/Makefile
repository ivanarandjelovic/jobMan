VPATH = src

CFLAGS = `pkg-config --cflags glib-2.0 glibmm-2.4 giomm-2.4 gtkmm-3.0 gconfmm-2.6`
LFLAGS = `pkg-config --libs glib-2.0 glibmm-2.4 giomm-2.4 gtkmm-3.0 gconfmm-2.6`

all : jobMan

objects = job.o jobManWindow.o main.o services.o

jobMan : $(objects)
	g++ -o $@ $(objects) $(LFLAGS)
		
#job.o : job.cpp job.h
#	g++ $(CFLAGS) -c $< 

%.cpp : %.h

#jobManWindow.o : jobManWindow.cpp jobManWindow.h services.h
#	g++ $(CFLAGS) -c $<

#jobManWindow.cpp : services.h

#services.o : services.cpp services.h job.h
#	g++ $(CFLAGS) -c $<

#services.cpp : job.h

#main.o : main.cpp services.h jobManWindow.h
#	g++ $(CFLAGS) -c $<

#.PHONY : main.cpp

#main.cpp : services.h jobManWindow.h

#Non standard prerequisites/inlcudes:

main.o jobManWindow.o : services.h
main.o : jobManWindow.h
services.o : job.h

%.o : %.cpp %.h
	g++ $(CFLAGS) -c $<

.PHONY : clean
clean : 
	rm -v -f jobMan $(objects)

