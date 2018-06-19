CC:=gcc

TARGET = server
OBJECT = $(TARGET).c

lp = -lpthread

all:$(TARGET)
$(TARGET):$(OBJECT)
	$(CC) -o $@ $(OBJECT) $(lp)

clean:
	rm -f *.o $(TARGET)
