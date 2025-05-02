#include"ultrasonic.h"
#include "gpio.h"
#include "util/delay.h"
#include"icu.h"

static volatile uint8  g_ICUFALLEDGE=0;







void Ultrasonic_init(void)
{

ICU_ConfigType a_ICU_attributes ={ICU_F_CPUOVER_8, RISINGEDGE};
ICU_init(&a_ICU_attributes);
ICU_setCallBack(Ultrasonic_edgeProcessing);
GPIO_setupPinDirection(PORTB_ID, PIN5_ID,PIN_OUTPUT);


}
void Ultrasonic_Trigger(void)
{
GPIO_writePin(PORTB_ID, PIN5_ID,LOGIC_HIGH);
_delay_us(15);
GPIO_writePin(PORTB_ID, PIN5_ID,LOGIC_LOW);
}
uint16 Ultrasonic_readDistance(void)
{

	Ultrasonic_Trigger();
	while(!g_ICUFALLEDGE);
	g_ICUFALLEDGE=0;

return (g_GETCOUNT/58.3 +1);
}
void Ultrasonic_edgeProcessing(void)
{
	static uint8 a_tick=0;
	a_tick++;

	if(a_tick==1)
	{
	ICU_setEdgeDetectionType(FALLINGEDGE);
	ICU_clearTimerValue();
	}
	else if(a_tick==2)
	{
		g_GETCOUNT=ICU_getInputCaptureValue();
		g_ICUFALLEDGE=1;
		a_tick=0;
		ICU_setEdgeDetectionType(RISINGEDGE);

	}


}


