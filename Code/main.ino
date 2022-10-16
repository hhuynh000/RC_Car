//#include "RR.h"
//#include "SRRI.h"
#include "DDS.h"

void setup() {
    setupTask1();
    setupSpeaker();
    setupSeg7();
    //setupSRRI();
    setupDDS();
}

void loop() {
    // DEMO 1
    //scheduler_RR();

    // DEMO 2/4
    //run_SRRI();

    // DEMO 3/5/6
    run_DDS();

}