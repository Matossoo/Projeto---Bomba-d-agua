 // ALTERADO: O sistema continua salvando na memória mesmo se estiver esvaziando com o LED apagado
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