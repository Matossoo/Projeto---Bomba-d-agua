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
