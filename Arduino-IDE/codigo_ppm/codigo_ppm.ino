// Variaveis
const int PINO_SENSOR_MQ2 = A0;

// formula usada = y = mx + b
float R0 = 0.60; // Resistência base do sensor (ar limpo)
float b = 1.413; // Coeficiente linear (interseção com o eixo y)
float m = -0.473; // Coeficiente angular (inclinação da reta)
// ambos 'b'e 'm' vêm do gráfico do datasheet do MQ-2

void setup() {
  Serial.begin(9600);
}

void loop() {
  int valorSensor = analogRead(PINO_SENSOR_MQ2); // Valor analógico do sensor que lê de 0 a 1023

  // Cálculo da resistência do sensor
  float valorSensor_Volts = (valorSensor * 5) / 1023.0; // Converte para tensão (0-5V)
  float RS = (5 - valorSensor_Volts) / valorSensor_Volts; // Resistência atual do sensor
  float razao = RS / R0; 

  // log10(x) = ln(x) * 1/ln(10)
  // Serie Taylor usada para chegar no valor aproximado de ln(x):
  // 2 * (u + 1/3*uˆ3)
  // u = (x-1)/(x+1)
  // limite (x ➡️ 2)
  float x = razao;
  float u = (x - 1) / (x + 1);
  float ln_x = 2 * (u + (u*u*u) / 3); // Achar o log natural da razão

  if (valorSensor > 950) {
    ln_x = ln_x * 1.5; // Como o código tende a ser menos preciso a partir de 950~960,
  }                    // e a diferenca entre esperado e o real é de 50%~,
                      // um ajuste manual é feito
  float log_razao = ln_x * 0.4343; // para entao converter para log10
  // 0.4343 é o valor aproximado de 1/ln(10)
  
  float PPM_Log = (log_razao - b) / m; // Equação da reta
  // PPM_Log == log10(PPM)

  // transformando potência de base 10 em potência de base e (Euler)
  // Baseado na Serie de Taylor para a formula e^x
  // eˆx = 1 + xˆn / n! + xˆn/ n+! ...
  // Para simulação, até o terceiro termo (n = 3) é o suficiente
  // Para maior precisão, utilizar o valor 2.302585092994046
  float expoente = 2.3026 * PPM_Log; // 2.3026 é o valor aproximado de ln(10) 
  float PPM = 1.0 + expoente + (expoente * expoente / 2.0) + (expoente * expoente * expoente) / 6.0;

  Serial.println(PPM);


  delay(2000);
}