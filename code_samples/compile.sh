g++ 001_opengl_stereo.cpp ../lib/libPAL.so  `pkg-config --libs --cflags opencv`   -lv4l2 -lpthread -O3 -o 001_opengl_stereo.out -I../include/  -lGL -lGLU -lX11 -lglut 

g++ 002_opengl_disparity.cpp ../lib/libPAL.so  `pkg-config --libs --cflags opencv`   -lv4l2 -lpthread -O3 -o 002_opengl_disparity.out -I../include/  -lGL -lGLU -lX11 -lglut 

