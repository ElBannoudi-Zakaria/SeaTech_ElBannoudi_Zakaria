/* 
 * File:   CB_TX1.h
 * Author: TP-EO-1
 *
 * Created on 3 f�vrier 2023, 16:04
 */

#ifndef CB_TX1_H
#define	CB_TX1_H

void SendMessage(unsigned char* message, int length);
void CB_TX1_Add(unsigned char value);
unsigned char CB_TX1_Get(void);
void SendOne();
void __attribute__((interrupt, no_auto_psv)) _U1TXInterrupt(void);
unsigned char CB_TX1_IsTranmitting(void);
int CB_TX1_GetDataSize(void);
int CB_TX1_GetRemainingSize(void);



#endif	/* CB_TX1_H */

