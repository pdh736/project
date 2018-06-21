CC:=gcc

TARGET = server
OBJECT = server_sock
SRC = src/
HEADER = -Iinc

lp = -lpthread

all:$(TARGET)
$(TARGET) : $(OBJECT).o $(TARGET).o
	$(CC) -o $@ $(TARGET).o $(OBJECT).o $(lp)
	rm -f *.o

$(OBJECT).o : $(SRC)$(OBJECT).c
	$(CC) -c -o $@ $< $(HEADER)

$(TARGET).o : $(SRC)$(TARGET).c
	$(CC) -c -o $@ $< $(HEADER)


clean:
	rm -f *.o $(TARGET)
