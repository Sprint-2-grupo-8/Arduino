const int PINO_SENSOR_MQ2 = A0;

// Variaveis
// formula usada = y = mx + b
float R0 = 0.48; // 
float b = 1.413; // interseção com o eixo y
float m = -0.473; // coeficiente angular


void setup() {
  Serial.begin(9600);
}

void loop() {
  int valorSensor = analogRead(PINO_SENSOR_MQ2);
  float valorSensor_Volts = (valorSensor * 5.0) / 1023.0;
  float RS = (5.0 - valorSensor_Volts) / valorSensor_Volts;
  float razao = RS / R0;
  float PPM_log = (log10(razao) - b) / m;
  float PPM = pow(10, PPM_Log);

  Serial.print("PPM: ");
  Serial.println(PPM);


  delay(1000);
}
