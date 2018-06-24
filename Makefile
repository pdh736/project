CC:=gcc

TARGET = server
OBJECT = server.o server_sock.o server_sql.o util.o
SOCK = server_sock
UTIL = util
SQL = server_sql
SRC = src/
HEADER = -Iinc -I/usr/include/mysql

lp = -lpthread -lmysqlclient


all:$(TARGET)
$(TARGET) : $(OBJECT)
	$(CC) -o $@ $^ $(lp)
	rm -f *.o

$(SOCK).o : $(SRC)$(SOCK).c
	$(CC) -c -o $@ $< $(HEADER)

$(UTIL).o : $(SRC)$(UTIL).c
	$(CC) -c -o $@ $< $(HEADER)

$(SQL).o : $(SRC)$(SQL).c
	$(CC) -c -o $@ $< $(HEADER)

$(TARGET).o : $(SRC)$(TARGET).c
	$(CC) -c -o $@ $< $(HEADER)


clean:
	rm -f *.o $(TARGET)
