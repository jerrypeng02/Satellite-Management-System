#include "commandManagement.h"
#include "constant.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/**
S
The S command indicates START mode. The command shall start the embedded
tasks by directing the hardware to initiate all the measurement tasks. In doing so, the
command shall enable all the interrupts.
P
The P command indicates STOP mode. This command shall stop the embedded tasks
by terminating any running measurement tasks. Such an action shall disable any data
collecting interrupts.
D
The D command enables or disables the display.
T<payload>
The T command transmits the thrust command to the satellite.
M<payload>
The M command. The M command requests the return of the most recent value(s) of
the specified data.
The M response. The M response returns of the most recent value(s) of the specified
data.
A <sp command>
The A response acknowledges the receipt of the identified command.
E
The E error response is given for incorrect commands or non-existent commands.
*/