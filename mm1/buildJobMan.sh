#!/bin/sh

g++ -o jobMan -m64 src/mm1.cpp src/mmWindow.cpp src/job.cpp src/services.cpp `pkg-config --libs --cflags glib-2.0 glibmm-2.4 giomm-2.4 gtkmm-3.0 gconfmm-2.6`
