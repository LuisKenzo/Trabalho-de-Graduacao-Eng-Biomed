   /*
  EMG-Muscular-Signal-Sensor
  made on 26 Jan 2021
  by Amir Mohammad Shojaee @ Electropeak
  Home

*/
int contracao = 160;
int LED = 13;
float media;
float fator = 0.05;

void setup() {
  Serial.begin(115200);
 // pinMode(LED, OUTPUT);
 // media = 0;
}

void loop() {
  float medida = analogRead(A0);
  //media = media *(1-fator) + medida*fator;

  Serial.print("Biceps D:");
  Serial.println(medida);
  //Serial.print(" ");
  //Serial.print(",");
  //Serial.print("\tVariable_2:");
  //Serial.print(contracao);
  //Serial.print("\tmedia:");
  //Serial.println(media);  

  //if(analogRead(A0) >= contracao){
  //  digitalWrite(LED, HIGH);
  //}
  //else {
  //  digitalWrite(LED, LOW);
  //}
  delay(100);
}
