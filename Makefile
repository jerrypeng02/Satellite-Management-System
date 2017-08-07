CC = gcc
CFlAGS = -g -I

HEADERS = constant.h util.h
OBJ = main.o bbb.o optfft.o consoleDisplay.o keyBoardConsole.o powerSubsystem.o imageCapture.o transportDistance.o satelliteComs.o solarPanelControl.o thrusterSubsystem.o util.o vehicleComms.o warningAlarm.o


%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)

vehicle_main: vehicle_main.o
	$(CC) -o $@ $^ $(CFLAGS)

main: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f *.o main


