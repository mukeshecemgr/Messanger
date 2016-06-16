CC= gcc
CFLAGS= -m32 -g -I/home/madcoder/Desktop/LAB/C/messanger/inc

objs: prep msg_main.o user.o server.o user_msg.o tx_thread.o rx_thread.o \
			server_msg_handler.o Decoder_APIs.o utils.o

#vpath %.o ./obj
.SILENT:
OBJ_DIR=/home/madcoder/Desktop/LAB/C/messanger/obj
SRC=/home/madcoder/Desktop/LAB/C/messanger/src
run: $(OBJ_DIR)/*.o
	$(CC) $(CFLAGS) -o $@ $^ -lpthread  -lrt  


all: run

prep:
	if [ ! -d ${OBJ_DIR} ] ; then \
	   mkdir ${OBJ_DIR} ;\
	fi 

msg_main.o:msg_main.c
	$(CC) $(CFLAGS) -c msg_main.c -o $(OBJ_DIR)/$@

user.o:$(SRC)/user.c
	$(CC) $(CFLAGS) -c $(SRC)/user.c -o $(OBJ_DIR)/$@

server.o:$(SRC)/server.c
	$(CC) $(CFLAGS) -c $(SRC)/server.c -o $(OBJ_DIR)/$@

utils.o:$(SRC)/utils.c
	$(CC) $(CFLAGS) -c $(SRC)/utils.c -o $(OBJ_DIR)/$@

user_msg.o:$(SRC)/user_msg.c
	$(CC) $(CFLAGS) -c $(SRC)/user_msg.c -o $(OBJ_DIR)/$@

tx_thread.o:$(SRC)/tx_thread.c
	$(CC) $(CFLAGS) -c $(SRC)/tx_thread.c -o $(OBJ_DIR)/$@

server_msg_handler.o:$(SRC)/server_msg_handler.c
	$(CC) $(CFLAGS) -c $(SRC)/server_msg_handler.c -o $(OBJ_DIR)/$@

rx_thread.o:$(SRC)/rx_thread.c
	$(CC) $(CFLAGS) -c $(SRC)/rx_thread.c -o $(OBJ_DIR)/$@

Decoder_APIs.o:$(SRC)/Decoder_APIs.c
	$(CC) $(CFLAGS) -c $(SRC)/Decoder_APIs.c -o $(OBJ_DIR)/$@

clean:
	rm -f $(OBJ_DIR)/*.o run
