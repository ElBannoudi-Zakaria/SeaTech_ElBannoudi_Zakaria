#ifndef PWM_H
#define PWM_H
#define MOTEUR_DROIT 0
#define MOTEUR_GAUCHE 1

void InitPWM(void);
//void PWMSetSpeed(float vitesseEnPourcents, unsigned char motorNumber);
void PWMSetSpeedConsigne( float vitesseEnPourcents, char moteur);
void PWMUpdateSpeed(void);

#endif