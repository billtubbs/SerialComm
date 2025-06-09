#ifndef SERIALCOMM_H
#define SERIALCOMM_H

#define START_MARKER 254
#define END_MARKER 255
#define SPECIAL_BYTE 253
#define MAX_PACKAGE_LEN 8192
#define MSG_BUFFER_SIZE 100

#include <Arduino.h>

extern uint16_t numBytesRecvd;
extern uint16_t numBytesExpected;

extern byte dataRecvd[MAX_PACKAGE_LEN];
extern byte dataSend[MAX_PACKAGE_LEN];
extern byte tempBuffer[MAX_PACKAGE_LEN * 2];

extern uint16_t dataRecvCount;
extern uint16_t dataSendCount; // number of data bytes to send to the PC
extern uint16_t dataTotalSend; // number of actual bytes to send to PC including encoded bytes

// Control logic for serial comms
extern boolean receivingInProgress;
extern boolean allReceived;
extern boolean connEstablished;

// For sending debug messages to PC
extern char msg_buffer[MSG_BUFFER_SIZE];

void newConnection(const char* myName);
void getSerialData();
void decodeHighBytes();
void dataToPC();
void encodeHighBytes();
void debugToPC(char arr[]);
void debugToPC(byte num);

#endif