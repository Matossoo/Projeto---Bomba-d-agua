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