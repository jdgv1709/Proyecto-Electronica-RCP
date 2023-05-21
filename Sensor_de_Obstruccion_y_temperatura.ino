//Juan David Galan Vargas 202111470
// Definir inputs
const int signalPin1 = A0;
const int signalPin2 = A1;
const int analogPin = A0;

// Definir output
const int outputPin = 13;

void setup() {

  Serial.begin(9600);

  // Ajustar output
  pinMode(outputPin, OUTPUT);
}

void loop() {
  // Leer las señales de input
  int signalValue1 = analogRead(signalPin1);
  int signalValue2 = analogRead(signalPin2);

  // Imprimir valores las señales y ploteandolas
  Serial.print(signalValue1);
  Serial.print(",");
  Serial.println(signalValue2);

  // Comparacion de las señales para activar o no el led
  if (signalValue1 < signalValue2) {
    digitalWrite(outputPin, HIGH);
  } else {
    digitalWrite(outputPin, LOW);
  }
  int analogValue = analogRead(analogPin);

  // Convert the analog value to temperature in degrees Celsius
  float temperature = (analogValue * 0.00488 - 0.5) * 10.0;

  // Print the temperature to the serial monitor
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" degrees Celsius");
  //espera para tomar otra medida
  delay(100);
}
