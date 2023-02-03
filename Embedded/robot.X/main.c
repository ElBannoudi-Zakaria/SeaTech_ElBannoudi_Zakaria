/* 
 * File:   main.c
 * Author: TP-EO-1
 *
 * Created on 21 septembre 2022, 15:06
 */
#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <libpic30.h>
#include <xc.h>
#include "ChipConfig.h"
#include "timer.h"
#include "IO.h"
#include "PWM.h"
#include "ADC.h"
#include "Robot.h"
#include "UART.h"

unsigned char stateRobot;
unsigned char stateSensor;

int vitesse = 30;

void OperatingSystemLoop(void) {
    switch (stateRobot) {
        case STATE_ATTENTE:
            timestamp = 0;
            PWMSetSpeedConsigne(0, MOTEUR_DROIT);
            PWMSetSpeedConsigne(0, MOTEUR_GAUCHE);
            stateRobot = STATE_ATTENTE_EN_COURS;

        case STATE_ATTENTE_EN_COURS:
            if (timestamp > 1000)
                stateRobot = STATE_AVANCE;
            break;

        case STATE_AVANCE:
            PWMSetSpeedConsigne(vitesse, MOTEUR_DROIT);
            PWMSetSpeedConsigne(vitesse, MOTEUR_GAUCHE);
            stateRobot = STATE_AVANCE_EN_COURS;
            break;
        case STATE_AVANCE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;

        case STATE_TOURNE_GAUCHE:
            PWMSetSpeedConsigne(vitesse, MOTEUR_DROIT);
            PWMSetSpeedConsigne(0, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_GAUCHE_EN_COURS;
            break;
        case STATE_TOURNE_GAUCHE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;

        case STATE_TOURNE_DROITE:
            PWMSetSpeedConsigne(0, MOTEUR_DROIT);
            PWMSetSpeedConsigne(vitesse, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_DROITE_EN_COURS;
            break;
        case STATE_TOURNE_DROITE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;

        case STATE_TOURNE_SUR_PLACE_GAUCHE:
            PWMSetSpeedConsigne(vitesse / 2, MOTEUR_DROIT);
            PWMSetSpeedConsigne(-vitesse / 2, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE_EN_COURS;
            break;
        case STATE_TOURNE_SUR_PLACE_GAUCHE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;

        case STATE_TOURNE_SUR_PLACE_DROITE:
            PWMSetSpeedConsigne(-vitesse / 2, MOTEUR_DROIT);
            PWMSetSpeedConsigne(vitesse / 2, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_SUR_PLACE_DROITE_EN_COURS;
            break;
        case STATE_TOURNE_SUR_PLACE_DROITE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;

        default:
            stateRobot = STATE_ATTENTE;
            break;
    }
}

unsigned char nextStateRobot = 0;

void SetNextRobotStateInAutomaticMode() {
    //    //
    //unsigned char positionObstacle = PAS_D_OBSTACLE;
    //
    ////Détermination de la position des obstacles en fonction des télémètres
    //if ( robotState.distanceTelemetreDroit < 30 &&
    //robotState.distanceTelemetreCentre > 20 &&
    //robotState.distanceTelemetreGauche > 30) //Obstacle à droite
    //positionObstacle = OBSTACLE_A_DROITE;
    //else if(robotState.distanceTelemetreDroit > 30 &&
    //robotState.distanceTelemetreCentre > 20 &&
    //robotState.distanceTelemetreGauche < 30) //Obstacle à gauche
    //positionObstacle = OBSTACLE_A_GAUCHE;
    //else if(robotState.distanceTelemetreCentre < 20) //Obstacle en face
    //positionObstacle = OBSTACLE_EN_FACE;
    //else if(robotState.distanceTelemetreDroit > 30 &&
    //robotState.distanceTelemetreCentre > 20 &&
    //robotState.distanceTelemetreGauche > 30) //pas d?obstacle
    //positionObstacle = PAS_D_OBSTACLE;
    //
    ////Détermination de l?état à venir du robot
    //if (positionObstacle == PAS_D_OBSTACLE)
    //nextStateRobot = STATE_AVANCE;
    //else if (positionObstacle == OBSTACLE_A_DROITE)
    //nextStateRobot = STATE_TOURNE_GAUCHE;
    //else if (positionObstacle == OBSTACLE_A_GAUCHE)
    //nextStateRobot = STATE_TOURNE_DROITE;
    //else if (positionObstacle == OBSTACLE_EN_FACE)
    //nextStateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE;
    //   //





    switch (stateSensor) {
        case 0b00000:
            //positionObstacle = PAS_D_OBSTACLE;
            nextStateRobot = STATE_AVANCE;
            break;
        case 0b00001:
            //positionObstacle = OBSTACLE_A_GAUCHE;
            nextStateRobot = STATE_TOURNE_DROITE;
            break;
        case 0b00010:
            //positionObstacle = OBSTACLE_A_GAUCHE;
            nextStateRobot = STATE_TOURNE_DROITE;
            break;
        case 0b00011:
            // positionObstacle = OBSTACLE_A_GAUCHE;
            nextStateRobot = STATE_TOURNE_DROITE;
            break;
        case 0b00100:
            //positionObstacle = OBSTACLE_EN_FACE;
            nextStateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE;
            break;
        case 0b00101:
            //positionObstacle = OBSTACLE_EN_FACE_GAUCHE;
            nextStateRobot = STATE_TOURNE_SUR_PLACE_DROITE;
            break;
        case 0b00110:
            //positionObstacle = OBSTACLE_EN_FACE_GAUCHE;
            nextStateRobot = STATE_TOURNE_SUR_PLACE_DROITE; //
            break;
        case 0b00111:
            //positionObstacle = OBSTACLE_EN_FACE_GAUCHE;
            nextStateRobot = STATE_TOURNE_SUR_PLACE_DROITE; //
            break;
        case 0b01000:
            //positionObstacle == OBSTACLE_A_DROITE;
            nextStateRobot = STATE_TOURNE_GAUCHE;
            break;
        case 0b01001:
            //positionObstacle = OBSTACLE_A_GAUCHE;
            nextStateRobot = STATE_TOURNE_SUR_PLACE_DROITE; //
            break;
        case 0b01010:
            //positionObstacle = OBSTACLE_EN_FACE;
            nextStateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE; //
            break;
        case 0b01011:
            //positionObstacle = OBSTACLE_A_GAUCHE;
            nextStateRobot = STATE_TOURNE_SUR_PLACE_DROITE; //
            break;
        case 0b01100:
            //positionObstacle = OBSTACLE_EN_FACE_DROITE;
            nextStateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE; //
            break;
        case 0b01101:
            //positionObstacle = OBSTACLE_EN_FACE_GAUCHE;
            nextStateRobot = STATE_TOURNE_SUR_PLACE_DROITE; //
            break;
        case 0b01110:
            //positionObstacle = OBSTACLE_EN_FACE;
            nextStateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE; //
            break;
        case 0b01111:
            //positionObstacle = OBSTACLE_A_GAUCHE;
            nextStateRobot = STATE_TOURNE_SUR_PLACE_DROITE;
            break;
        case 0b10000:
            //positionObstacle = OBSTACLE_A_GAUCHE;
            nextStateRobot = STATE_TOURNE_GAUCHE;
            break;
        case 0b10001:
            //positionObstacle = OBSTACLE_A_GAUCHE;
            nextStateRobot = STATE_AVANCE;
            break;
        case 0b10010:
            //p/ositionObstacle = OBSTACLE_A_GAUCHE;
            nextStateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE;
            break;
        case 0b10011:
            //positionObstacle = OBSTACLE_A_GAUCHE;
            nextStateRobot = STATE_TOURNE_SUR_PLACE_DROITE; //
            break;
        case 0b10100:
            //positionObstacle = OBSTACLE_A_GAUCHE;
            nextStateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE; //
            break;
        case 0b10101:
            //positionObstacle = OBSTACLE_A_GAUCHE;
            nextStateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE;
            break;
        case 0b10110:
            //positionObstacle = OBSTACLE_A_GAUCHE;
            nextStateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE; //
            break;
        case 0b10111:
            // positionObstacle = OBSTACLE_A_GAUCHE;
            nextStateRobot = STATE_TOURNE_SUR_PLACE_DROITE;
            break;
        case 0b11000:
            // positionObstacle = OBSTACLE_A_GAUCHE;
            nextStateRobot = STATE_TOURNE_GAUCHE; //
            break;
        case 0b11001:
            //positionObstacle = OBSTACLE_A_GAUCHE;
            nextStateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE; //
            break;
        case 0b11010:
            // positionObstacle = OBSTACLE_A_GAUCHE;
            nextStateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE; //
            break;
        case 0b11011:
            //positionObstacle = OBSTACLE_A_GAUCHE;
            nextStateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE;
            break;
        case 0b11100:
            //positionObstacle = OBSTACLE_A_GAUCHE;
            nextStateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE; //
            break;
        case 0b11101:
            //positionObstacle = OBSTACLE_A_GAUCHE;
            nextStateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE;
            break;
        case 0b11110:
            //positionObstacle = OBSTACLE_A_GAUCHE;
            nextStateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE;
            break;
        case 0b11111:
            //positionObstacle = OBSTACLE_A_GAUCHE;
            nextStateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE;
            break;
    }

    //Si l?on n?est pas dans la transition de l?étape en cours
    if (nextStateRobot != stateRobot - 1)
        stateRobot = nextStateRobot;
}

int main(void) {

    //? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ?
    // I n i t i a l i s a t i o n   de   l ? o s c i l l a t e u r
    //? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ?
    InitOscillator();
    //? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ?
    // C o n f i g u r a t i o n   d e s   é e n t r e s   s o r t i e s
    //? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ?
    InitIO();

    InitTimer23();
    InitTimer1(50);
    InitTimer4(1000);

    LED_BLANCHE = 1;
    LED_BLEUE = 1;
    LED_ORANGE = 1;

    InitPWM();
    InitADC1();
    InitUART();
    // Boucle P r i n c i p a l e    
    while (1) {
        /*if (ADCIsConversionFinished() == 1) {
            ADCClearConversionFinishedFlag();
            unsigned int * result = ADCGetResult();
            ADCValue0 = result[0];
            ADCValue1 = result[1];
            ADCValue2 = result[2];}*/

        if (ADCIsConversionFinished() == 1) {
            ADCClearConversionFinishedFlag();
            unsigned int * result = ADCGetResult();

            float volts = ((float) result[0])*3.3 / 4096 * 3.2;
            robotState.distanceTelemetreExtDroit = 34 / volts - 5;

            volts = ((float) result[1]) * 3.3 / 4096 * 3.2;
            robotState.distanceTelemetreDroit = 34 / volts - 5;

            volts = ((float) result[2]) * 3.3 / 4096 * 3.2;
            robotState.distanceTelemetreCentre = 34 / volts - 5;

            volts = ((float) result[4])*3.3 / 4096 * 3.2;
            robotState.distanceTelemetreGauche = 34 / volts - 5;

            volts = ((float) result[3])*3.3 / 4096 * 3.2;
            robotState.distanceTelemetreExtGauche = 34 / volts - 5;


            stateSensor = 0;
            if (robotState.distanceTelemetreExtGauche < 20) {
                stateSensor = stateSensor | 0b00001;
            }
            if (robotState.distanceTelemetreGauche < 25) {
                stateSensor = stateSensor | 0b00010;
            }
            if (robotState.distanceTelemetreCentre < 25) {
                stateSensor = stateSensor | 0b00100;
            }
            if (robotState.distanceTelemetreDroit < 25) {
                stateSensor = stateSensor | 0b01000;
            }
            if (robotState.distanceTelemetreExtDroit < 20) {
                stateSensor = stateSensor | 0b10000;
            }



            if (robotState.distanceTelemetreGauche < 20 || robotState.distanceTelemetreExtGauche < 20) {
                LED_BLANCHE = 1;
            } else {
                LED_BLANCHE = 0;
            }
            if (robotState.distanceTelemetreCentre < 20) {
                LED_BLEUE = 1;
            } else {
                LED_BLEUE = 0;
            }
            if (robotState.distanceTelemetreDroit < 20 || robotState.distanceTelemetreExtDroit < 20) {
                LED_ORANGE = 1;
            } else {
                LED_ORANGE = 0;
            }
        }



        /*    if ( ADCValue0 >= 353) {
                LED_BLANCHE=1;
            }else{
                LED_BLANCHE=0;
            }
         
            if ( ADCValue1 >= 353) {
                LED_BLEUE=1;
            }else{
                LED_BLEUE=0;
            }
        
            if ( ADCValue2 >= 353) {
                LED_ORANGE=1;
            }else{
                LED_ORANGE=0;
            }*/
        SendMessageDirect((unsigned char*) "Bonjour", 7);
        __delay32(40000000);


    }


} // f i n main


