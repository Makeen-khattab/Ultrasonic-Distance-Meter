#include "icu.h"
#include "common_macros.h"
#include "gpio.h"
#include "avr/interrupt.h"


static volatile void (*g_callBackPtr)(void)=NULL;

void ICU_init(const ICU_ConfigType * Config_Ptr)
{
	/* Define the ICU PIN AS AN INPUT PIN*/

	GPIO_setupPinDirection(PORTD_ID,PIN6_ID, PIN_INPUT);


	/* The FOC1A/FOC1B bits are only active when we choose a non-PWM mode.*/
	SET_BIT(TCCR1A,FOC1A);
	SET_BIT(TCCR1A,FOC1B);

	/* OC1A/OC1B disconnected*/
	CLEAR_BIT(TCCR1A, COM1A0);
	CLEAR_BIT(TCCR1A, COM1A1);

	/* Choosing the mode of operation to be Normal to have the biggest range of counting 0 to 2^16-1 */
	CLEAR_BIT(TCCR1A, WGM10);
	CLEAR_BIT(TCCR1A, WGM11);
	CLEAR_BIT(TCCR1B, WGM12);
	CLEAR_BIT(TCCR1B, WGM13);

	/* Selecting the prescaler */

	TCCR1B = (TCCR1B &(0xF8)) | ((Config_Ptr->clock) &(0x07));

	/* Choosing the Edge Type*/
		TCCR1B = (TCCR1B & 0xBF) | (((Config_Ptr->edge) & 0x01)<<6);
	/* Enabling the ICU Peripheral Enable */

	SET_BIT( TIMSK, TICIE1);

	/* Setting an initial value for Timer1 register*/
	TCNT1=0;

	/* Setting an initial value for ICU register*/
	ICR1=0;


}

void ICU_setCallBack(void(*a_ptr)(void))
{

	g_callBackPtr = a_ptr;

}
void ICU_setEdgeDetectionType(const ICU_EdgeType edgeType)
{
	/* Choosing the Edge Type*/
	TCCR1B = (TCCR1B & 0xBF) | ((edgeType & 0x01)<<6);

}

uint16 ICU_getInputCaptureValue(void)
{
	return ICR1;
}

void ICU_clearTimerValue(void)
{
	TCNT1=0;
}
void ICU_deInit(void)
{
	TCCR1A=0;
	TCCR1B=0;
	TCNT1=0;
	ICR1=0;
	CLEAR_BIT( TIMSK, TICIE1);

	g_callBackPtr = NULL;

}

ISR(TIMER1_CAPT_vect)
{
	if(g_callBackPtr != NULL)
	{
		(*g_callBackPtr)();

	}

}


