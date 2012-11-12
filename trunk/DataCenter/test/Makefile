CXXFLAGS =	-O2 -g -Wall -I../ -fmessage-length=0

CPPSOCKET_SRC = $(wildcard cppsocket/*.cpp)

CPPSOCKET_OBJ = $(patsubst %.cpp, %.o, $(CPPSOCKET_SRC))


OBJS =		$(CPPSOCKET_OBJ)   $(patsubst %.cpp, %.o, $(wildcard *.cpp))

LIBS =   -lev

TARGET =	DataCenter

#$(TARGET):	$(OBJS)  


Packet_test:	../Packet.cpp Packet_test.cpp ../TraceLog.cpp
	$(CXX) $(CXXFLAGS) -o $@ -DPACKET_TEST $^

all:	Packet_test

clean:
	rm -f $(OBJS) $(TARGET)
