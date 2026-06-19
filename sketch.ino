#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <math.h>

#define PINO_DHT 2
#define TIPO_DHT DHT22
#define PINO_LUZ A0
#define LED_VERDE 8
#define LED_AMARELO 9
#define LED_VERMELHO 10
#define BUZZER 11

DHT dht(PINO_DHT, TIPO_DHT);
LiquidCrystal_I2C lcd(0x27, 16, 2);
Adafruit_MPU6050 mpu;

int calcularLuminosidade() {
  int leitura = analogRead(PINO_LUZ);
  int luminosidade = map(leitura, 0, 1023, 100, 0);
  return constrain(luminosidade, 0, 100);
}

float calcularVibracao() {
  sensors_event_t aceleracao;
  sensors_event_t giroscopio;
  sensors_event_t temperaturaSensor;

  mpu.getEvent(&aceleracao, &giroscopio, &temperaturaSensor);

  float aceleracaoTotal = sqrt(
    aceleracao.acceleration.x * aceleracao.acceleration.x +
    aceleracao.acceleration.y * aceleracao.acceleration.y +
    aceleracao.acceleration.z * aceleracao.acceleration.z
  );

  return abs(aceleracaoTotal - 9.81);
}

int verificarNivel(float temperatura, int luminosidade, float vibracao) {
  if (temperatura < 15 || temperatura > 35 ||
      luminosidade < 5 || luminosidade > 95 || vibracao > 3.0) {
    return 2;
  }

  if (temperatura < 18 || temperatura > 30 ||
      luminosidade < 20 || luminosidade > 85 || vibracao > 1.0) {
    return 1;
  }

  return 0;
}

void controlarAlertas(int nivel) {
  digitalWrite(LED_VERDE, nivel == 0);
  digitalWrite(LED_AMARELO, nivel == 1);
  digitalWrite(LED_VERMELHO, nivel == 2);

  if (nivel == 2) {
    tone(BUZZER, 1000);
  } else {
    noTone(BUZZER);
  }
}

String nomeDoNivel(int nivel) {
  if (nivel == 0) {
    return "NORMAL";
  } else if (nivel == 1) {
    return "ATENCAO";
  } else {
    return "CRITICO";
  }
}

void mostrarNoDisplay(float temperatura, int luminosidade, float vibracao, int nivel) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("T:");
  lcd.print(temperatura, 1);
  lcd.print("C L:");
  lcd.print(luminosidade);
  lcd.print("%");

  lcd.setCursor(0, 1);
  lcd.print("V:");
  lcd.print(vibracao, 1);
  lcd.print(" ");
  lcd.print(nomeDoNivel(nivel));
}

void mostrarNoMonitorSerial(float temperatura, int luminosidade, float vibracao, int nivel) {
  Serial.println("--------------------------------");
  Serial.print("Temperatura: ");
  Serial.print(temperatura, 1);
  Serial.println(" C");
  Serial.print("Luminosidade: ");
  Serial.print(luminosidade);
  Serial.println(" %");
  Serial.print("Vibracao: ");
  Serial.print(vibracao, 2);
  Serial.println(" m/s2");
  Serial.print("Situacao da capsula: ");
  Serial.println(nomeDoNivel(nivel));
}

void setup() {
  Serial.begin(115200);
  dht.begin();

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("CAPSULA ORION");
  lcd.setCursor(0, 1);
  lcd.print("Iniciando...");

  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_AMARELO, OUTPUT);
  pinMode(LED_VERMELHO, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  if (!mpu.begin()) {
    lcd.clear();
    lcd.print("Erro no MPU6050");
    Serial.println("MPU6050 nao encontrado.");
    while (true) {
      delay(100);
    }
  }

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  delay(1500);
}

void loop() {
  float temperatura = dht.readTemperature();
  int luminosidade = calcularLuminosidade();
  float vibracao = calcularVibracao();

  if (isnan(temperatura)) {
    Serial.println("Erro na leitura do DHT22.");
    delay(2000);
    return;
  }

  int nivel = verificarNivel(temperatura, luminosidade, vibracao);

  controlarAlertas(nivel);
  mostrarNoDisplay(temperatura, luminosidade, vibracao, nivel);
  mostrarNoMonitorSerial(temperatura, luminosidade, vibracao, nivel);

  delay(2000);
}
