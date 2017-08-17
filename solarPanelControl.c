#include "solarPanelControl.h"
#include "constant.h"
#include "stdio.h"
#include "bbb.h"
#include <signal.h>

// solar panel control function
unsigned short solarPanelProgress = 0;

void solarPanelControl(void* data) {
    if (taskCounter % MINOR_CYCLE_NUM_IN_MAJOR != 0)
        return;

    Bool* solarPanelState = ((SolarPanelControlData*)data)->solarPanelState;
    Bool* solarPanelDeploy = ((SolarPanelControlData*)data)->solarPanelDeploy;
    Bool* solarPanelRetract = ((SolarPanelControlData*)data)->solarPanelRetract;
    Bool* dmsInc = ((SolarPanelControlData*)data)->dmsInc;
    Bool* dmsDec = ((SolarPanelControlData*)data)->dmsDec;

    static unsigned short motorDrive = 0;


    if(*dmsInc){
        if(motorDrive <100){
            motorDrive += 5;
        }else{
            motorDrive = 100;
        }
    }
    if(*dmsDec){
        if(motorDrive > 0){
            motorDrive -= 5;
        }else{
            motorDrive = 0;
        }
    }
    if(*solarPanelDeploy){
        solarPanelProgress += motorDrive;
    }
    if(*solarPanelRetract){
        solarPanelProgress -= motorDrive;
    }

    if(solarPanelProgress >= 100){
        *solarPanelState = TRUE;
        solarPanelProgress = 100;
    }
    if(solarPanelProgress <= 0){
        *solarPanelState = FALSE;
        solarPanelProgress = 0;
    }

    if(*solarPanelDeploy || *solarPanelRetract){
        enablePWM();
        generatePWM(motorDrive);
    }

    if(*solarPanelState && *solarPanelDeploy){
        motorDrive = 0;
        *solarPanelDeploy = FALSE;
        isrNum = 3;
        signal(SIGUSR1);
        disablePWM();
    }
    if(!*solarPanelState && *solarPanelRetract){
        motorDrive = 0;
        *solarPanelRetract = FALSE;
        isrNum = 3;
        signal(SIGUSR1);
        disablePWM();
    }


    /*
    int pwm = bbb_enablePwm(HEADER, PIN);
    int pwmPeriod = bbb_setPwmPeriod(HEADER, PIN, PERIOD);
    int pwmDuty = bbb_setPwmDuty(HEADER, PIN, motorDrive * PERIOD / 100);
    */

    printf("motor %d\n", motorDrive);


    /*
    Bool pwm;

    int counter = 0;

        if(counter <= motorDrive){
            pwm = TRUE;
        }else{
            pwm = FALSE;
        }

        if(dmsInc){
            motorDrive += 5;
        }
        if(dmsDec){
            motorDrive -= 5;
        }

        counter++;
        if(counter > 100){
            counter = 0;
        }

    solarPanelDeploy = &pwm;
    solarPanelRetract = &pwm;
    */

}

void enablePWMforSolarPanelControl() {
    FILE *pwm;
	pwm = fopen("/sys/devices/bone_capemgr.9/slots", "w");
	fseek(pwm,0,SEEK_SET);
	fprintf(pwm,"am33xx_pwm");
	fflush(pwm);

	fprintf(pwm,"bone_pwm_P9_14");
	fflush(pwm);
	fclose(pwm);
}

void generatePWM(unsigned short speed){
    FILE *duty,*period;

	period = fopen("/sys/devices/ocp.3/pwm_test_P9_14.16/period", "w");
	fseek(period,0,SEEK_SET);
	fprintf(period,"%d",500000000);
	fflush(period);

	duty = fopen("/sys/devices/ocp.3/pwm_test_P9_14.16/duty", "w");
	fseek(duty,0,SEEK_SET);
	fprintf(duty,"%d",speed * (500000000/ 100));
	fflush(duty);

	fclose(duty);
	fclose(period);
}

void enablePWM() {
    FILE *run;
	run = fopen("/sys/devices/ocp.3/pwm_test_P9_14.16/run", "w");

	fseek(run,0,SEEK_SET);
	fprintf(run,"%d",1);
	fflush(run);
	fclose(run);
}

void disablePWM(){
    FILE *run;
	run = fopen("/sys/devices/ocp.3/pwm_test_P9_14.16/run", "w");

	fseek(run,0,SEEK_SET);
	fprintf(run,"%d",0);
	fflush(run);
	fclose(run);
}

