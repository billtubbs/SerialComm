#include <Arduino.h>
#include <SerialComm.h>

// Device names used for serial communications
#define MY_NAME "MyArduino"

// Function prototypes
void flashBoardLed();
void processData();

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT); // onboard LED

  // The board LED will flash until a connection is established.
  digitalWrite(LED_BUILTIN, HIGH);
}

void loop()
{
  if (Serial)
  {
    if (!connEstablished)
    {
      newConnection(MY_NAME);
      connEstablished = true;
    }
    getSerialData();
    processData();
  }
  else
  {
    connEstablished = false;
    flashBoardLed();
  }
}

void flashBoardLed()
{
  if ((millis() % 1000) > 100)
  {
    digitalWrite(LED_BUILTIN, LOW);
  }
  else
  {
    digitalWrite(LED_BUILTIN, HIGH);
  }
}

void processData()
{
  // Process the data that is in dataRecvd[]

  if (allReceived)
  {
    // Here is where you put your code to process the data received.
    // Instead, for this example, simply copy dataRecvd to dataSend
    // and send it back to the PC.
    dataSendCount = dataRecvCount;
    for (uint16_t n = 0; n < dataRecvCount; n++)
    {
      dataSend[n] = dataRecvd[n];
    }
    // digitalWrite(LED_BUILTIN, HIGH);
    dataToPC();
    allReceived = false;
  }
}