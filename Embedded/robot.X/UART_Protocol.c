#include <xc.h>
#include "UART_Protocol.h"

unsigned char UartCalculateChecksum(int msgFunction,int msgPayloadLength, unsigned char* msgPayload)
{
    //Fonction prenant entree la trame et sa longueur pour calculer le checksum
    unsigned char checksum = 0;
    checksum ^= 0xFE;
    checksum ^= (msgFunction >> 8);
    checksum ^= (msgFunction >> 0);
    checksum ^= (msgPayloadLength >> 8);
    checksum ^= (msgPayloadLength >> 0);
    for (int i = 0; i < msgPayloadLength; i++)
    {
        checksum ^= msgPayload[i];

    }
    return checksum;
    
}
void UartEncodeAndSendMessage(int msgFunction,int msgPayloadLength, unsigned char* msgPayload)
{
    //Fonction d?encodage et d?envoi d?un message
            unsigned char* msg[msgPayloadLength + 6];
            msg[0] = 0xFE;
            msg[1] = (unsigned char)(msgFunction >> 8);
            msg[2] = (unsigned char)(msgFunction >> 0);
            msg[3] = (unsigned char)(msgPayloadLength >> 8);
            msg[4] = (unsigned char)(msgPayloadLength >> 0);
            for (int i = 0; i < msgPayloadLength; i++)
            {
                msg[5 + i] = msgPayload[i];
            }
            msg[5 + msgPayloadLength] = UartCalculateChecksum(msgFunction, msgPayloadLength, msgPayload);
            SendMessageDirect(msg, msgPayloadLength + 6);
            //serialPort1.Write(msg, 0, msg.Length);
            //IR_Droit.Content += "ok";//Encoding.UTF8.GetString(msg,0,msg.Length);
}

int msgDecodedFunction = 0;
int msgDecodedPayloadLength = 0;
unsigned char msgDecodedPayload[128];
int msgDecodedPayloadIndex = 0;

void UartDecodeMessage(unsigned char c)
{
    //Fonction prenant en entree un octet et servant a reconstituer les trames
    switch (rcvState)
            {
                case StateReception.Waiting:
                    if (c == 0xFE)
                    {
                        rcvState = StateReception.FunctionMSB;
                    }
                    break;
                case StateReception.FunctionMSB:
                    msgDecodedFunction = c << 8;
                    rcvState = StateReception.FunctionLSB;
                    break;

                case StateReception.FunctionLSB:
                    msgDecodedFunction += c << 0;
                    rcvState = StateReception.PayloadLengthMSB;
                    break;
                case StateReception.PayloadLengthMSB:
                    msgDecodedPayloadLength = c << 8;
                    rcvState = StateReception.PayloadLengthLSB;
                    break;
                case StateReception.PayloadLengthLSB:
                    msgDecodedPayloadLength += c << 0;
                    if (msgDecodedPayloadLength == 0)
                        rcvState = StateReception.CheckSum;
                    else
                    {
                        rcvState = StateReception.Payload;
                        msgDecodedPayloadIndex = 0;
                        msgDecodedPayload = new byte[msgDecodedPayloadLength];
                    }
                    break;
                case StateReception.Payload:
                    msgDecodedPayload[msgDecodedPayloadIndex] = c;
                    msgDecodedPayloadIndex++;
                    if (msgDecodedPayloadIndex >= msgDecodedPayloadLength)
                        rcvState = StateReception.CheckSum;
                    break;

                case StateReception.CheckSum:
                    int calculatedChecksumvaleur = CalculateChecksum(msgDecodedFunction, msgDecodedPayloadLength, msgDecodedPayload);
                    int receivedChecksum = c;
                    if (calculatedChecksumvaleur == receivedChecksum)
                    {
                        textBoxReception.Text += "Message bien reçu";
                        ProcessDecodedMessage(msgDecodedFunction, msgDecodedPayloadLength, msgDecodedPayload);

                    }
                    else
                        textBoxReception.Text += "Message corrompu";
                    rcvState = StateReception.Waiting;
                    break;
                default:
                    rcvState = StateReception.Waiting;
                    break;
            }
}
void UartProcessDecodedMessage(int function,int payloadLength, unsigned char* payload)
{
    //Fonction appelee apres le decodage pour executer l?action
    //correspondant au message recu
    switch (Function)
            {
                case 0x0080:
                    textBoxReception.Text = Encoding.ASCII.GetString(msgPayload);
                    break;
                case 0x0020:
                    if (msgPayload[0] == (byte)(1))
                        if (msgPayload[1] == (byte)(1))
                            CheckBoxLed1.IsChecked = true;
                        else
                            CheckBoxLed1.IsChecked = false;
                    else if (msgPayload[0] == (byte)(2))
                        if (msgPayload[1] == (byte)(1))
                            CheckBoxLed2.IsChecked = true;
                        else
                            CheckBoxLed2.IsChecked = false;
                    else 
                        if (msgPayload[1] == (byte)(1))
                            CheckBoxLed3.IsChecked = true;
                        else
                            CheckBoxLed3.IsChecked = false;
                    break;
                case 0x0030:

                    Label_IR_Gauche.Content = msgPayload[0];
                    Label_IR_Centre.Content = msgPayload[1];
                    Label_IR_Droit.Content = msgPayload[2];
                    break;
                case 0x0040:
                    Label_VitesseGauche.Content= msgPayload[0];
                    Label_VitesseDroit.Content = msgPayload[1];


                    break;
            }
}
//*************************************************************************/
//Fonctions correspondant aux messages
//*************************************************************************/


        