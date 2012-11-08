CXXFLAGS =	-O2 -g -Wall -fmessage-length=0

CPPSOCKET_SRC = $(wildcard cppsocket/*.cpp)

CPPSOCKET_OBJ = $(patsubst %.cpp, %.o, $(CPPSOCKET_SRC))


OBJS =		$(CPPSOCKET_OBJ)   $(patsubst %.cpp, %.o, $(wildcard *.cpp))

LIBS =   -lev

TARGET =	DataCenter

$(TARGET):	$(OBJS)  
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)

all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
