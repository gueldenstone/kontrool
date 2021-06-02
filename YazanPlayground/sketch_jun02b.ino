unsigned long myTime;

void setup() {
  Serial.begin(9600);
}
void loop() {
  Serial.print("Time: ");
  myTime = millis();

  Serial.println(myTime); // prints time since program started
  delay(1000);          // wait a second so as not to send massive amounts of data
}
