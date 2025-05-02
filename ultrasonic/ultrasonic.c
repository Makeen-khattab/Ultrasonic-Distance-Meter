#include"ultrasonic.h"
#include "gpio.h"
#include "util/delay.h"


g_ICUFALLEDGE=0;
g_GETCOUNT;







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
GPIO_writePin(PORTB_ID, PIN5_ID,LOGIC_HIGH);
}
uint16 Ultrasonic_readDistance(void)
{


	while(!g_ICUFALLEDGE);
	g_ICUFALLEDGE=0;

return g_GETCOUNT/58.8;
}
void Ultrasonic_edgeProcessing(void)
{
	static uint8 a_tick=0;
	a_tick++;

	if(a_tick==1)
	ICU_setEdgeDetectionType(FALLINGEDGE);
	else if(a_tick==2)
	{
		g_GETCOUNT=ICU_getInputCaptureValue();
		g_ICUFALLEDGE=1;
	}


}


