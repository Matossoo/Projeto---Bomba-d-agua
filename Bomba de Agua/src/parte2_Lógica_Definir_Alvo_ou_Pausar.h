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
