//Variables voltaje y corriente
#define pin_sensor A0 //Pin del sensor
float adc; //Variable para obtener los valores en el 1 paso
float voltaje; //Variable para obtener el voltaje en el 2 paso
float v_in;
//Variables de envío
int8_t trPower = 1;         // Transreceiver power  ( can be -3 to 15)
String SprFactor = "sf12";  // Spreadingsfactor     (can be sf7 to sf12)
uint8_t max_dataSize = 100; // Maximum charcount to avoid writing outside of string
unsigned long readDelay = 60000; // Time to read for messages in ms (max 4294967295 ms, 0 to disable)

const char CR = '\r';
const char LF = '\n';

void setup() {
  // put your setup code here, to run once:
  SerialUSB.begin(57600);
  Serial2.begin(57600);

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LED_GREEN, OUTPUT) ;

  while (!SerialUSB && millis() < 1000);

  LoraP2P_Setup();
  digitalWrite(LED_BUILTIN, HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:
  
  // Some data to send
  //char Data[100] = "48656c6c6f20576f726c6421";
//Paso 1, conversión ADC de la lectura del pin analógico
  adc = analogRead(pin_sensor);
  //Serial2.println(adc);

  //Paso 2, obtener el voltaje
  voltaje = adc * 25 / 1023;
 // Serial.println(voltaje);
  delay(1000);

  LORA_Write(voltaje);
  digitalWrite(LED_GREEN, LOW); // To let us know when the data is send
  delay(100);
  digitalWrite(LED_GREEN, HIGH);


}

void LoraP2P_Setup()
{

  Serial2.print("sys reset\r\n");
  delay(200);
  Serial2.print("radio set pwr ");
  Serial2.print(trPower);
  Serial2.print("\r\n");
  delay(100);
  Serial2.print("radio set sf ");
  Serial2.print(SprFactor);
  Serial2.print("\r\n");
  delay(100);
  Serial2.print("radio set wdt ");
  Serial2.print(readDelay);
  Serial2.print("\r\n");
  delay(100);
  Serial2.print("mac pause\r\n");
  delay(100);

  FlushSerialBufferIn();
}

void FlushSerialBufferIn()
{
  while (Serial2.available() > 0)
  {
#ifdef DEBUG
    SerialUSB.write(Serial2.read());
#else
    Serial2.read();
#endif
  }
}

void LORA_Write(float Data)
{
  Serial2.print("radio tx ");
  Serial2.print(Data);
  Serial2.print("\r\n");
  Serial2.flush();

  waitTillMessageGone();

}

// Waits until the data transmit is done
void waitTillMessageGone()
{
  while (!Serial2.available());
  delay(10);
  while (Serial2.available() > 0)
    Serial2.read();

  while (!Serial2.available());
  delay(10);
  while (Serial2.available() > 0)
  {
#ifdef DEBUG
    SerialUSB.write(Serial2.read());
#else
    Serial2.read();
#endif
  }
}
