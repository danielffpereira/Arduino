// Distributed with a free-will license.
// Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
// AD5669
// This code is designed to work with the AD5669_I2CDAC I2C Mini Module available from ControlEverything.com.
// https://www.controleverything.com/content/Digital-Analog?sku=AD5669_I2CDAC#tabs-0-product_tabset-2

#include<Wire.h>

const byte interruptPin_Trigger = 26;

// AD5669 I2C address is 0x56(86)
#define Addr 0x56

void setup()
{
  // Initialise I2C communication as Master
  Wire.begin();


  const byte interruptPin_Trigger = 26;
  // Initialise serial communication, set baud rate = 9600
  attachInterrupt(digitalPinToInterrupt(interruptPin_Trigger), triggerFunction , RISING);
  Serial.begin(115200);
}

bool state = true;
bool stateT = false;
unsigned int data[2] = {0x00, 0x00};;
void loop()
{
  if (stateT) {
    if (state) {
      data[0] = 0x00;
      data[1] = 0x00;
      state = false;
    } else {
      data[0] = 0xac;
      data[1] = 0x00;
      state = true;
    }
    // Start I2C transmission
    Wire.beginTransmission(Addr);
    Wire.write(0);
    Wire.endTransmission();
    Wire.beginTransmission(Addr);
    Wire.write(0x30);
    Wire.write(data[0]);
    Wire.write(data[1]);
    Wire.endTransmission();

    // Convert the data, Vref = 5 V
    float voltage = (((data[0] * 256) + (data[1])) / 65536.0) * 5.0;
    // Output data to serial monitor
    Serial.print("Voltage : ");
    Serial.print(voltage);
    Serial.println(" V");
  }
}
void triggerFunction()
{
  stateT = true;
}
