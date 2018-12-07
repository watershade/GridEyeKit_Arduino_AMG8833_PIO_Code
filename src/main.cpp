#include <Arduino.h>
#include <Wire.h>
#include "grideye.h"
//#include "GE_SoftUart.h"
//#include "GE_SoftUartParse.h"

/*******************************************************************************
  variable value definition
*******************************************************************************/
static uint16_t Main_Delay = 85;
grideye GE_GridEyeSensor;
uint8_t aucThsBuf[2];   /* thermistor temperature        */
short g_ashRawTemp[64]; /* temperature of 64 pixels      */


/******************************************************************************
  Function：GE_SentDataToPC
  Description：Used to Sent data to PC
  Input：None
  Output：None
  Others：None
******************************************************************************/
void GE_SentDataToPC(void)
{
  Serial.print("***");
  Serial.write(aucThsBuf[0]);
  Serial.write(aucThsBuf[1]);
  for (int i = 0; i < 128; i++)
  {
    Serial.write(*((uint8_t *)(g_ashRawTemp) + i));
  }
  Serial.print("\r\n");
}

/******************************************************************************
  Function：GE_SentDatatoPC
  Description：Used to Sent data to PC
  Input ：None
  Output：None
  Others：None 
******************************************************************************/
void GE_SourceDataInitialize(void)
{
  for (int i = 0; i < 64; i++)
  {
    g_ashRawTemp[i] = 0xAAAA;
  }
}

/******************************************************************************
  Function：GE_UpdateFerquency
  Description：Used to set the Grid-EYE update frequency
  Input：None
  Output：Grid-EYE frequency
  Return：None
  Others：None
******************************************************************************/
void GE_UpdateFerquency(uint8_t GE_SetFrequency)
{
  switch (GE_SetFrequency)
  {
  case 10: /* set update frequency 10Hz */
  {
    Main_Delay = 85;
  }
  break;

  case 1: /* set update frequency 1Hz */
  {

    Main_Delay = 985;
  }
  break;

  default:
    break;
  }
}


void setup() {
  Serial.begin(57600);

  /* Initialize Grid-Eye data interface */
  GE_GridEyeSensor.init(1);

  /* Initialize variables  */
  GE_SourceDataInitialize();


}

void loop() {
  /* Get thermistor register value. */
  GE_GridEyeSensor.bAMG_PUB_I2C_Read(0x0E, 2, aucThsBuf);

  /* Get temperature register value. */
  for (int i = 0; i < 4; i++)
  {
    GE_GridEyeSensor.bAMG_PUB_I2C_Read(0x80 + 32 * i, 32, (uint8_t *)g_ashRawTemp + i * 32);
  }

  /* Send Grid-Eye sensor data to PC */
  GE_SentDataToPC();

  /* set update frequency */
  GE_UpdateFerquency(10);

  delay(Main_Delay);


}