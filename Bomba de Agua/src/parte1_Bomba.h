#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

int nivelPins[5] = {8, 9, 10, 11, 12};
int vazaoPin = A0;
int ledBombaPin = 13; 

long consumo = 0;
bool enchendo = false;

int nivelTravado = 0;
int nivelAlvo = 0;

// Variável para a "Detecção de Borda" dos botões
bool estadoAnterior[5] = {LOW, LOW, LOW, LOW, LOW};

// EEPROM endereços
int addrConsumo = 0;
int addrNivel = 10;

void setup() {
  lcd.init(); // Se no VS Code der erro, troque por lcd.begin()
  lcd.backlight();
  Serial.begin(9600);

  for(int i = 0; i < 5; i++){
    pinMode(nivelPins[i], INPUT);
  }
  
  pinMode(ledBombaPin, OUTPUT); 
  digitalWrite(ledBombaPin, LOW); 

  // Recupera dados salvos
  EEPROM.get(addrConsumo, consumo);
  EEPROM.get(addrNivel, nivelAlvo);
}

void loop() {
  int novoNivel = 0;

  // 1. LEITURA DOS BOTÕES
  for(int i = 0; i < 5; i++){
    bool estadoAtual = digitalRead(nivelPins[i]);
    
    if(estadoAtual == HIGH && estadoAnterior[i] == LOW){
      switch(i + 1){
        case 1: novoNivel = 10; break;
        case 2: novoNivel = 30; break;
        case 3: novoNivel = 50; break;
        case 4: novoNivel = 70; break;
        case 5: novoNivel = 90; break;
      }
    }
    estadoAnterior[i] = estadoAtual; 
  }
