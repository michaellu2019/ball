#ifndef MILLISECONDS_H_
#define MILLISECONDS_H_
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
    
// the following counter can be used to start things on 1 millisecond boundaries
extern volatile uint16 ms, seconds, minutes, hours;    
extern volatile uint32 MILLISECONDS;
 
// initialize function
void init_milliseconds();

#endif
/* [] END OF FILE */