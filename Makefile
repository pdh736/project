CC:=gcc

TARGET = server
OBJECT = server_sock
UTIL = util
SRC = src/
HEADER = -Iinc -I/usr/include/mysql

lp = -lpthread -lmysqlclient


all:$(TARGET)
$(TARGET) : $(OBJECT).o $(TARGET).o $(UTIL).o
	$(CC) -o $@ $^ $(lp)
	rm -f *.o

$(OBJECT).o : $(SRC)$(OBJECT).c
	$(CC) -c -o $@ $< $(HEADER)

$(UTIL).o : $(SRC)$(UTIL).c
	$(CC) -c -o $@ $< $(HEADER)

$(TARGET).o : $(SRC)$(TARGET).c
	$(CC) -c -o $@ $< $(HEADER)


clean:
	rm -f *.o $(TARGET)
