CC = gcc
CFLAGS = -g

HEADERS = constant.h util.h
OBJ = main.o bbb.o optfft.o consoleDisplay.o keyBoardConsole.o powerSubsystem.o imageCapture.o transportDistance.o satelliteComs.o solarPanelControl.o thrusterSubsystem.o util.o vehicleComms.o warningAlarm.o webserver.o


%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)

vehicle_main: vehicle_main.o
	$(CC) -o $@ $^ $(CFLAGS)

webserver_main: webserver_main.o webserver.o
	$(CC) -o $@ $^ $(CFLAGS) -L/usr/local/lib -lmicrohttpd

main: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) -L/usr/local/lib -lmicrohttpd

.PHONY: clean

clean:
	rm -f *.o main


