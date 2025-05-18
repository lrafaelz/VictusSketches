int EMGSensor() {
	// read the analog in value:
	sensorValue = analogRead(analogInPin);
	// map it to the range of the analog out:
	outputValue = map(sensorValue, 0, 4095, 0, 255);
	// change the analog out value:
	analogWrite(analogOutPin, outputValue);
  return sensorValue;

}
// code from Arduino IDE: File > Examples > 03.Analogic > AnalogicInOutSerial
