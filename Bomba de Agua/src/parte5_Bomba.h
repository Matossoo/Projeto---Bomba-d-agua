 
 // 5. GUARDAR NA EEPROM (Agora a cada 0.5 segundos)
  // 500 milissegundos = 0.5 segundos
  if (enchendo && (millis() - tempoUltimoSave >= 500)) {
    EEPROM.put(addrConsumo, consumo);
    EEPROM.put(addrNivel, nivelAlvo);
    tempoUltimoSave = millis();
  }
  
  // Garante que guarda também no exato momento em que pausar ou atingir a meta
  static bool estadoAnteriorEnchendo = false;
  if (estadoAnteriorEnchendo == true && enchendo == false) {
    EEPROM.put(addrConsumo, consumo);
    EEPROM.put(addrNivel, nivelAlvo);
    tempoUltimoSave = millis(); // Reinicia o cronómetro
  }
  estadoAnteriorEnchendo = enchendo;