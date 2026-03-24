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

int nivelAlvo = 0;

// Variável para a "Detecção de Borda" dos botões
bool estadoAnterior[5] = {LOW, LOW, LOW, LOW, LOW};

// EEPROM endereços e controlo de tempo
int addrConsumo = 0;
int addrNivel = 10;
unsigned long tempoUltimoSave = 0;

void setup() {
  lcd.init(); // Troque por lcd.begin() se der erro no VS Code
  lcd.backlight();
  Serial.begin(9600);

  for(int i = 0; i < 5; i++){
    pinMode(nivelPins[i], INPUT);
  }
  
  pinMode(ledBombaPin, OUTPUT); 
  digitalWrite(ledBombaPin, LOW); 

  // Recupera dados guardados da memória
  EEPROM.get(addrConsumo, consumo);
  EEPROM.get(addrNivel, nivelAlvo);

  // Trava de segurança: Se a memória estiver corrompida ou virgem de fábrica
  if(consumo < 0 || consumo > 10000 || nivelAlvo < 0 || nivelAlvo > 100){
    consumo = 0;
    nivelAlvo = 0;
  }
}

void loop() {
  int novoNivel = 0;

  // 1. LEITURA DOS BOTÕES (Apenas no clique)
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

  // 2. LÓGICA DE DEFINIR ALVO OU PAUSAR
  if(novoNivel > 0){
    
    // REGRA 1: NÃO PODE DIMINUIR
    if(novoNivel < nivelAlvo){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("ERRO:");
      lcd.setCursor(0,1);
      lcd.print("Nivel menor!");
      delay(2000);
      return; 
    }
    
    // REGRA 3: PAUSAR OU RETOMAR O NÍVEL ATUAL
    else if(novoNivel == nivelAlvo){
      long metaLitros = nivelAlvo * 100;
      if (consumo < metaLitros) { 
        enchendo = !enchendo; 
      }
    }
    
    // REGRA 4: AVANÇAR DE NÍVEL (Apenas em sequência)
    else if(novoNivel > nivelAlvo){
      
      int nivelAnteriorNecessario = 0;
      switch(novoNivel){
        case 10: nivelAnteriorNecessario = 0; break;
        case 30: nivelAnteriorNecessario = 10; break;
        case 50: nivelAnteriorNecessario = 30; break;
        case 70: nivelAnteriorNecessario = 50; break;
        case 90: nivelAnteriorNecessario = 70; break;
      }

      long litrosNecessarios = nivelAnteriorNecessario * 100;

      if (nivelAlvo == nivelAnteriorNecessario && consumo >= litrosNecessarios) {
        nivelAlvo = novoNivel;
        enchendo = true;
      } else {
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("ERRO: Sequencia");
        lcd.setCursor(0,1);
        lcd.print("Invalida!");
        delay(2000);
        return; 
      }
    }
  }

  long metaLitros = nivelAlvo * 100;

  // 3. CALCULAR CONSUMO
  if(enchendo){
    int vazao = analogRead(vazaoPin);
    int incremento = map(vazao, 0, 1023, 0, 100);
    consumo += incremento;
  }

  // Parar ao atingir a meta
  if(enchendo && consumo >= metaLitros){
    enchendo = false;
    consumo = metaLitros; 
  }

  // 4. ATUALIZAR O LED (Bomba)
  if(enchendo){
    digitalWrite(ledBombaPin, HIGH); 
  } else {
    digitalWrite(ledBombaPin, LOW);  
  }

  // 5. GUARDAR NA EEPROM (Agora a cada 0.5 segundos)
  // 500 milissegundos = 0.5 segundos
 //ALTERADO: O sistema continua salvando na memória mesmo se estiver esvaziando com o LED apagado
  bool sistemaAtivo = (enchendo || esvaziando);
  
  if (sistemaAtivo && (millis() - tempoUltimoSave >= 500)) {
    EEPROM.put(addrConsumo, consumo);
    EEPROM.put(addrNivel, nivelAlvo);
    tempoUltimoSave = millis();
  }
  
  static bool estadoAnteriorSistema = false;
  if (estadoAnteriorSistema == true && sistemaAtivo == false) {
    EEPROM.put(addrConsumo, consumo);
    EEPROM.put(addrNivel, nivelAlvo);
    tempoUltimoSave = millis(); 
  }
  estadoAnteriorSistema = sistemaAtivo;

  // 6. DISPLAY (Ecrã)
  lcd.clear();

  lcd.setCursor(0,0);
  lcd.print("Nivel:");
  lcd.print(nivelAlvo);
  lcd.print("%");

  lcd.setCursor(0,1);
  lcd.print("Cons:");
  lcd.print(consumo);
  lcd.print(" L");

  // Nota: Como o delay do ecrã é 500ms, o Arduino irá verificar a gravação da memória
  // sensivelmente a cada ciclo do loop enquanto a bomba estiver ativa.
  delay(500); 
}