void preTransmission()
{
  digitalWrite(MAX485_RE_NEG, 1);
  digitalWrite(MAX485_DE, 1);
}

void postTransmission()
{
  digitalWrite(MAX485_RE_NEG, 0);
  digitalWrite(MAX485_DE, 0);
}

void readSensor()
{
  result = node.readInputRegisters(0x0001, 2);
  if (result == node.ku8MBSuccess)
  {
    temp = node.getResponseBuffer(0) / 10.0f;
    hum = node.getResponseBuffer(1) / 10.0f;
//    Serial.print("Temp: "); Serial.print(temp); Serial.print("\t");
//    Serial.print("Hum: "); Serial.print(hum);
//    Serial.println();
    delay(1000);
  }
}
