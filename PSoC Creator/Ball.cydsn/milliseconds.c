/* ========================================
 * Copyright Wade Maxfield, 2020
 * All Rights Reserved
 * LICENSED SOFTWARE.
 *  Under the GPL v3 license
 * This license does not override previous licenses
 * Some information may be Proprietary to 
 * Cypress (http://www.cypress.com) for their
 * PSoC 5LP®--Cypress Semiconductor and
 * only usable on their devices.
 * PROPERTY OF Wade Maxfield.
 * Commercial license available
 * ========================================
*/
#include "project.h"

volatile uint32 MILLISECONDS;
volatile uint16 ms, seconds, minutes, hours;

CY_ISR(Millisecond_Interrupt) {
    MILLISECONDS++;
    
    if (++ms >= 1000){
        ms = 0;
        if (++seconds >= 60){
            seconds = 0;
            if (++minutes >= 60) {
                minutes = 0;
                if (++hours >= 24){
                    hours = 0;
                }
            }
        }
    }
}

void init_milliseconds() {
    Clock_Millis_Interrupt_StartEx(Millisecond_Interrupt);
}


/* [] END OF FILE */