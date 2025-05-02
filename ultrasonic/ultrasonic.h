#ifndef ULTRASONIC_H_
#define ULTRASONIC_H_
#include "std_types.h"

#define SOUNDSPEED						343


uint8 g_ICUFALLEDGE;
uint8 g_GETCOUNT;
void Ultrasonic_init(void);
void Ultrasonic_Trigger(void);
uint16 Ultrasonic_readDistance(void);
void Ultrasonic_edgeProcessing(void);

#endif /* ULTRASONIC_H_ */
