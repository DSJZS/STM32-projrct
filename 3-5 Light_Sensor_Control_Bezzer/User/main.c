#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "Buzzer.h"
#include "LightSensor.h"

int main(void)
{
	Buzzer_Init();
	LightSensor_Init();
	while(1)
	{
		if( 1 == LightSensor_Get() )
			Buzzer_ON();
		else
			Buzzer_OFF();
	}
}
