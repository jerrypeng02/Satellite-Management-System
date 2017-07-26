CC = gcc
CFlAGS = -I

HEADERS = constant.h util.h
OBJ = main.o consoleDisplay.o keyBoardConsole.o powerSubsystem.o satelliteComs.o solarPanelControl.o thrusterSubsystem.o util.o vehicleComms.o warningAlarm.o


%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)

main: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) 
.PHONY: clean

clean:
	rm -f *.o main


