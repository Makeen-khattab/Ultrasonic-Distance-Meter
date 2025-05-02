#include "lcd.h"
#include "gpio.h"
#include "Common_Macros.h"
#include <util/delay.h>

static void LCD_integerToString(uint8 *string_Ptr,uint32 a_num);
void LCD_init(void)
{
	GPIO_setupPinDirection(LCD_RS_PORTID, LCD_RS_PINID, PIN_OUTPUT);
GPIO_setupPinDirection(LCD_EN_PORTID, LCD_EN_PINID, PIN_OUTPUT);
#if(LCD_8BITSMODE==1)
GPIO_setupPortDirection(LCD_DATA_PORTID, PORT_OUTPUT);


#elif(LCD_4BITSMODE ==1 )
GPIO_setupPinDirection(LCD_DATA_PORTID, LCD_PIN0ID, PIN_OUTPUT);
GPIO_setupPinDirection(LCD_DATA_PORTID, LCD_PIN1ID, PIN_OUTPUT);
GPIO_setupPinDirection(LCD_DATA_PORTID, LCD_PIN2ID, PIN_OUTPUT);
GPIO_setupPinDirection(LCD_DATA_PORTID, LCD_PIN3ID, PIN_OUTPUT);
#endif
/* as the LCD takes 15 millisecond to start up	*/
	_delay_ms(20);
#if(LCD_8BITSMODE==1)
LCD_sendCommand(LCD_EIGHTBIT_TWOLINE);
#elif(LCD_4BITSMODE ==1 )
LCD_sendCommand(LCD_TWO_LINES_FOUR_BITS_MODE_INIT1 );
LCD_sendCommand(LCD_TWO_LINES_FOUR_BITS_MODE_INIT2 );
LCD_sendCommand(LCD_FOURBIT_TWOLINE);
#endif

	LCD_sendCommand(LCD_DISPLAYON_CURSOROFF	);
	LCD_sendCommand(LCD_CLEAR_DISPLAY);
}

void LCD_sendCommand(uint8 a_LCD_commands)
{
	/* RS bit is responsible for register select for each of data and
	 *  commands to adjust settings*/
	GPIO_writePin(LCD_RS_PORTID,LCD_RS_PINID, LCD_SELECT_COMMAND_REGISTER);
		_delay_ms(1);
	/* EN bit is used to turn on the screen*/
	GPIO_writePin(LCD_EN_PORTID,LCD_EN_PINID,LOGIC_HIGH);
		_delay_ms(1);
#if(LCD_8BITSMODE==1)
		GPIO_writePort(LCD_DATA_PORTID,a_LCD_commands);
#elif(LCD_4BITSMODE ==1 )
		GPIO_writePin(LCD_DATA_PORTID,LCD_PIN0ID,GET_BIT(a_LCD_commands,4));
		GPIO_writePin(LCD_DATA_PORTID,LCD_PIN1ID,GET_BIT(a_LCD_commands,5));
		GPIO_writePin(LCD_DATA_PORTID,LCD_PIN2ID,GET_BIT(a_LCD_commands,6));
		GPIO_writePin(LCD_DATA_PORTID,LCD_PIN3ID,GET_BIT(a_LCD_commands,7));
		_delay_ms(1);
		GPIO_writePin(LCD_EN_PORTID,LCD_EN_PINID,LOGIC_LOW);
		_delay_ms(1);
		GPIO_writePin(LCD_EN_PORTID,LCD_EN_PINID,LOGIC_HIGH);

		GPIO_writePin(LCD_DATA_PORTID,LCD_PIN0ID,GET_BIT(a_LCD_commands,0));
		GPIO_writePin(LCD_DATA_PORTID,LCD_PIN1ID,GET_BIT(a_LCD_commands,1));
		GPIO_writePin(LCD_DATA_PORTID,LCD_PIN2ID,GET_BIT(a_LCD_commands,2));
		GPIO_writePin(LCD_DATA_PORTID,LCD_PIN3ID,GET_BIT(a_LCD_commands,3));
#endif
		_delay_ms(1);
		GPIO_writePin(LCD_EN_PORTID,LCD_EN_PINID,LOGIC_LOW);
		_delay_ms(1);
}

void LCD_displayCharachter(uint8 a_char)
{
	/* RS bit is responsible for register select for each of data and
		 *  commands to adjust settings*/
		GPIO_writePin(LCD_RS_PORTID,LCD_RS_PINID,LCD_SELECT_DATA_REGISTER);
			_delay_ms(1);
		/* EN bit is used to turn on the screen*/

		GPIO_writePin(LCD_EN_PORTID,LCD_EN_PINID,LOGIC_HIGH);
			_delay_ms(1);
#if(LCD_8BITSMODE==1)
		GPIO_writePort(LCD_DATA_PORTID,a_char);
#elif(LCD_4BITSMODE ==1 )
		GPIO_writePin(LCD_DATA_PORTID,LCD_PIN0ID,GET_BIT(a_char,4));
		GPIO_writePin(LCD_DATA_PORTID,LCD_PIN1ID,GET_BIT(a_char,5));
		GPIO_writePin(LCD_DATA_PORTID,LCD_PIN2ID,GET_BIT(a_char,6));
		GPIO_writePin(LCD_DATA_PORTID,LCD_PIN3ID,GET_BIT(a_char,7));
		_delay_ms(1);
		GPIO_writePin(LCD_EN_PORTID,LCD_EN_PINID,LOGIC_LOW);
		_delay_ms(1);
		GPIO_writePin(LCD_EN_PORTID,LCD_EN_PINID,LOGIC_HIGH);

		GPIO_writePin(LCD_DATA_PORTID,LCD_PIN0ID,GET_BIT(a_char,0));
		GPIO_writePin(LCD_DATA_PORTID,LCD_PIN1ID,GET_BIT(a_char,1));
		GPIO_writePin(LCD_DATA_PORTID,LCD_PIN2ID,GET_BIT(a_char,2));
		GPIO_writePin(LCD_DATA_PORTID,LCD_PIN3ID,GET_BIT(a_char,3));
#endif
			_delay_ms(1);
			GPIO_writePin(LCD_EN_PORTID,LCD_EN_PINID,LOGIC_LOW);
			_delay_ms(1);
}

void LCD_displayString(uint8 * a_char)
{
	uint8*a_charPTR=a_char;

		while( *(a_charPTR)!= '\0')
		{

			LCD_displayCharachter(*a_charPTR);
			a_charPTR++;
		}
}

void LCD_moveCursor(uint8 a_row,uint8 a_col)
{
	uint8 a_command=0;
if((a_row>MAXROWPOS)|(a_row<MINROWPOS) | (a_col>MAXCOLPOS)|(a_col<MINCOLPOS) )
{
	/* Do nothing*/
}
else
{
		switch (a_row)
		{
		case 1 :
			a_command=FIRSTROWADD;
			break;
		case 2 :
			a_command=SECROWADD;
			break;
#if(LCD_FOURLINE)
		case 3:
			a_command=THIRDROWADD;
			break;
		case 4 :
			a_command=FOURTHROWADD;
			break;
#endif

		}

LCD_sendCommand((a_command | (a_col-1)|(LCD_SET_CURSOR_LOCATION)));

}
}

void LCD_displayStringRowColumn(uint8 a_row,uint8 a_column,uint8  * a_char)
{
	LCD_moveCursor(a_row,a_column);
	LCD_displayString(a_char);
}
void LCD_clearScreen()
{

	LCD_sendCommand(LCD_CLEAR_DISPLAY);

}

void LCD_intgerToString(uint32 a_num)
{
	uint8 buff[16];
	LCD_integerToString(buff,a_num);

	LCD_displayString(buff);

}

static void LCD_integerToString(uint8 *  string_Ptr,uint32 a_num)
{ uint8 i=0,temp=0;
uint8 *Ptr_Begin=string_Ptr, *Ptr_End=string_Ptr;


	while(a_num !=0 && i<15)
	{
		*(string_Ptr +i)=  (((0x0F) & (a_num%10)) | ZEROASCII);

		a_num/=10;
		i++;
	}

	*(string_Ptr +i)='\0';

	while(*Ptr_End!= '\0')
	{
		Ptr_End++;

	}
	Ptr_End--;

	while(Ptr_End-Ptr_Begin > 0)
	{
		temp= *Ptr_End;
		*Ptr_End=*Ptr_Begin;
		*Ptr_Begin=temp;
		Ptr_End--;
		Ptr_Begin++;
	}



}

