CXXFLAGS =	-O2 -g -Wall -fmessage-length=0

OBJS =		DataCenter.o

LIBS =

TARGET =	DataCenter

$(TARGET):	$(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)

all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
