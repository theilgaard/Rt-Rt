OBJS = build/Camera.o\
       build/Config.o\
       build/Main.o\
       build/RTRT.o\
       build/ShaderManager.o
CC = g++
DEBUG = -g
CFLAGS = -Wall -c -std=c++11 -MD
LFLAGS = -Wall `pkg-config --static --libs glew glfw3`
PROGRAMNAME = Rtrt.out
OUTPUTDIR = .
SHADERDIR = $(OUTPUTDIR)/shaders/

$(PROGRAMNAME) : $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o $(PROGRAMNAME)
	mkdir $(SHADERDIR)
	cp src/shaders/* $(SHADERDIR)

$(OBJS): build/%.o: src/%.cpp
	$(CXX) $(CFLAGS) -c $< -o $@

clean: 
	rm build/*.o $(OUTPUTDIR)/$(PROGRAMNAME) 
	rm -rf $(SHADERDIR) 
