CXX=g++
CXXFLAGS=`pkg-config --cflags gtkglextmm-1.2`
LFLAGS=`pkg-config --libs gtkglextmm-1.2`
OBJS=src/main.o src/MengerGL.o
EXEC=menger

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CXX) $(LFLAGS) $^ -o $@

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $< -o $@

clean:
	rm -f $(EXEC) $(OBJS)

.PHONY: all clean
