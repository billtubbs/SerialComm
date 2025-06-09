#include "SerialComm.h"
//#define DEBUG_MODE

// TODO: consider making some of these locals?
uint16_t numBytesRecvd = 0;
uint16_t numBytesExpected = 0;

byte dataRecvd[MAX_PACKAGE_LEN];
byte dataSend[MAX_PACKAGE_LEN];
byte tempBuffer[MAX_PACKAGE_LEN * 2];  // this buffer will contain additional special bytes

uint16_t dataRecvCount = 0;
uint16_t dataSendCount = 0; // number of data bytes to send to the PC
uint16_t dataTotalSend = 0; // number of actual bytes to send to PC including encoded bytes

// Control logic for serial comms
boolean connEstablished = false;
boolean receivingInProgress = false;
boolean allReceived = false;
// boolean sendingInProgress = false;  // TODO: Add non-blocking data sending
// boolean allSent = false;

// For sending debug messages to PC
char msg_buffer[MSG_BUFFER_SIZE];

void newConnection(const char* myName)
{
    snprintf(msg_buffer, MSG_BUFFER_SIZE, "My name is %s", myName);
    debugToPC(msg_buffer);
    digitalWrite(LED_BUILTIN, LOW);
}

void getSerialData()
{
    /* Receives data from serial connection into tempBuffer[] then calls
     * decodeHighBytes() to decode and copy data from tempBuffer[] to
     * dataRecvd[] for use by the Arduino program.
     */

    if (Serial.available() > 0)
    {

        byte x = Serial.read();

        if (!receivingInProgress)
        {
            if (x == START_MARKER)
            {
                numBytesRecvd = 0;
                receivingInProgress = true;
            }
        }
        else
        {
            if (numBytesRecvd >= MAX_PACKAGE_LEN * 2)
            {
                receivingInProgress = false;
                snprintf(
                    msg_buffer,
                    MSG_BUFFER_SIZE,
                    "getSerialData failed: number of bytes exceeds %d",
                    MAX_PACKAGE_LEN * 2);
                debugToPC(msg_buffer);
            }
            else
            {
                if (x != END_MARKER)
                {
                    tempBuffer[numBytesRecvd] = x;
                    numBytesRecvd++;
                }
                else
                {
                    receivingInProgress = false;

                    // Decode the data from tempBuffer[] into dataRecvd[]
                    decodeHighBytes();
                    if (dataRecvCount >= MAX_PACKAGE_LEN)
                    {
                        snprintf(msg_buffer, MSG_BUFFER_SIZE, "Num. of data bytes exceeds buffer size %d", MAX_PACKAGE_LEN);
                        debugToPC(msg_buffer);
                    }
                    allReceived = true;

                    #ifdef DEBUG_MODE
                    snprintf(msg_buffer, MSG_BUFFER_SIZE, "Total actual bytes received: %d.", numBytesRecvd);
                    debugToPC(msg_buffer);
                    snprintf(msg_buffer, MSG_BUFFER_SIZE, "Num. of data bytes received: %d.", dataRecvCount);
                    debugToPC(msg_buffer);
                    #endif

                }
            }
        }
    }
}

void decodeHighBytes()
{
    /*  copies the length data in the first two bytes and the data bytes to
     *  dataRecvd[], converting any bytes following the special byte into
     *  the intended values.
     *  Note that numBytesRecvd is the total of all the bytes including the
     *  special bytes.
     */
    dataRecvCount = 0;
    for (uint16_t n = 0; n < numBytesRecvd; n++)
    {
        byte x = tempBuffer[n];
        if (x == SPECIAL_BYTE)
        {
            n++;
            x = x + tempBuffer[n];
        }
        if (dataRecvCount >= MAX_PACKAGE_LEN)
        {
            break;
        }
        dataRecvd[dataRecvCount] = x;
        dataRecvCount++;
    }
}

void dataToPC()
{
    /* expects to find data in dataSend[]
     * uses encodeHighBytes() to copy data to tempBuffer
     * sends data to PC from tempBuffer
     */
    encodeHighBytes();
    Serial.write(START_MARKER);
    Serial.write(tempBuffer, dataTotalSend);
    Serial.write(END_MARKER);
}

void encodeHighBytes()
{
    /* Copies to tempBuffer[] all of the data in dataSend[]
     * and converts any bytes of 253 or more into a pair of
     * bytes, 253 0, 253 1 or 253 2 as appropriate.
     */
    dataTotalSend = 0;
    for (uint16_t n = 0; n < dataSendCount; n++)
    {
        if (dataSend[n] >= SPECIAL_BYTE)
        {
            tempBuffer[dataTotalSend] = SPECIAL_BYTE;
            dataTotalSend++;
            tempBuffer[dataTotalSend] = dataSend[n] - SPECIAL_BYTE;
        }
        else
        {
            tempBuffer[dataTotalSend] = dataSend[n];
        }
        dataTotalSend++;
    }
}

void debugToPC(char arr[])
{
    byte nb = 0;
    Serial.write(START_MARKER);
    Serial.write(nb);
    Serial.write(nb);
    Serial.print(arr);
    Serial.write(END_MARKER);
}

void debugToPC(byte num)
{
    byte nb = 0;
    Serial.write(START_MARKER);
    Serial.write(nb);
    Serial.write(nb);
    Serial.print(num);
    Serial.write(END_MARKER);
}