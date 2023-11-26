#include "Servo.h"
#include "stm32f10x.h"                  // Device header
#include "PWM.h"

void Servo_Init(void)
{
	PWM_Init();
}

// 0° 	- 	500
// 180°	- 	2500
void Servo_SetAngle(float Angle)
{
	PWM_SetCpmpare2( Angle/180*2000+500 );
}
