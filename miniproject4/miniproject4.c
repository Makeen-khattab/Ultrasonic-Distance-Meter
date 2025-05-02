#include "miniproject4.h"


int main(void)
{
	SREG |= (1<<7);
	LCD_init();
	uint16 distance;
	Ultrasonic_init();
while(1)
{

distance= Ultrasonic_readDistance();

LCD_moveCursor(1,1);
LCD_displayString("Distance= ");
LCD_intgerToString(distance);
LCD_displayString("cm ");


}

}
