/*
 * Only one mouse died during this project
 */

int clickPin = 2;
int sensorPin = A0;
int threeshold = 500;
unsigned long timer = 0;
bool flag = false;

void setup() {
  Serial.begin(115200);
  pinMode(clickPin, OUTPUT);
  pinMode(sensorPin, INPUT);
  digitalWrite(clickPin, HIGH);

}

void loop() {
  int value = analogRead(sensorPin);
  Serial.println(value);
  if (value > threeshold && !flag) {
    flag=true;
    timer=millis();
    digitalWrite(clickPin, LOW);
    delay(500);
    digitalWrite(clickPin, HIGH);
  }
  if(millis()-timer>2000){ // give time to the sensor to stop vibrating
    flag=false;
  }
  
}
