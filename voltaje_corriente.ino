#define pin_sensor_v A0 //Pin del sensor
#define pin_sensor_i A1
float adc; //Variable para obtener los valores en el 1 paso
float voltaje; //Variable para obtener el voltaje en el 2 paso
float v_in;
float volt_I;
float corriente;
float Sensibilidad=0.0631;
float corriente_suma, corriente_env, counter;
void setup() {
  Serial.begin(9600);

}

void loop() {
  //Paso 1, conversión ADC de la lectura del pin analógico
  adc = analogRead(pin_sensor_v);
  //Paso 2, obtener el voltaje
  voltaje = adc * 25 / 1023;
  
  while(counter < 300){
    volt_I = analogRead(pin_sensor_i)*(5.0 / 1023.0); //lectura del sensor   
    corriente =(volt_I-2.5197)/Sensibilidad; //EcuaciÃ³n  para obtener la corriente
    corriente_suma = corriente_suma+corriente;
    counter ++;
    delay(100); 
  }
  corriente_env = (corriente_suma/counter)-0.5;
  counter = 0;
  
  
  Serial.print(adc);
  Serial.print("\t");
  Serial.println(voltaje);
  Serial.print("Corriente: ");
  Serial.println(corriente,3); 

}
