CXXFLAGS =	-O2 -g -Wall -fmessage-length=0

CPPSOCKET_SRC = $(wildcard cppsocket/*.cpp)

CPPSOCKET_OBJ = $(patsubst %.cpp, %.o, $(CPPSOCKET_SRC))


OBJS =		$(CPPSOCKET_OBJ)   $(patsubst %.cpp, %.o, $(wildcard *.cpp))

LIBS =   -lev

TARGET =	DataCenter

#$(TARGET):	$(OBJS)  

$(TARGET):	$(CPPSOCKET_SRC) $(wildcard *.cpp)
	$(CXX) -o $(TARGET) $^ $(LIBS)


all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
