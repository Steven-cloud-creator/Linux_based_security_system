CROSS_COMPILE=arm-linux-gnueabihf-
CC=$(CROSS_COMPILE)gcc
CFLAGS= -c -g
LDFLAGS= -lpthread

OBJS=main.o data_global.o pthread_refresh.o pthread_transfer.o pthread_client.o pthread_dac.o

monitor_obj :$(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)
	mv *o ./obj
$(OBJS):%.o:%.c
	$(CC) $(CFLAGS) $< -o $@

.PHONY:clean
clean:
	rm *.o monitor_obj -rf 