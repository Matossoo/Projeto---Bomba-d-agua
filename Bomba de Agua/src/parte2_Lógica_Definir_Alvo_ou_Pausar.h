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