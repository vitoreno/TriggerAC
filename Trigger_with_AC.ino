// Trigger_with_AC.ino
// Sketch for triggering a camera in accordance with light wavering.
//
// If you use this code or get inspired by it, please cite:
// V. Renò, R. Marani, M. Nitti, N. Mosca, T. D'Orazio and E. Stella
// A Powerline-Tuned Camera Trigger For AC Illumination Flickering Reduction
// IEEE Embedded Systems Letters
// Print ISSN: 1943-0663
// Online ISSN: 1943-0671
// Digital Object Identifier: 10.1109/LES.2017.2755443

int sensorPin;  // Input pin for the AC
const int FirstPin =  9;  // First trigger output pin
const int LastPin =  13;  // Last trigger output pin

int sensorValue;
int OldSensorValue;
bool bStartTrigger;
unsigned long T1;
unsigned long T2;

void setup() {
  sensorPin = A0;
  for (int i = FirstPin; i <= LastPin; i++)  {
    pinMode(i, OUTPUT);
    digitalWrite(i, LOW);
  }
  Serial.begin(115200);
  OldSensorValue = analogRead(sensorPin);
  bStartTrigger = false;
  T1 = 2500;  // Wait time before trigger signal goes HIGH
  T2 = 5000;  // HIGH trigger signal time
}

void loop() {
  sensorValue = analogRead(sensorPin);
  if (bStartTrigger && (OldSensorValue <= 512 &&  OldSensorValue < sensorValue && sensorValue >= 512))  {
    delayMicroseconds(T1);
    for (int i = FirstPin; i <= LastPin; i++)
      digitalWrite(i, HIGH);
    
    delayMicroseconds(T2);
    
    for (int i = FirstPin; i <= LastPin; i++)
      digitalWrite(i, LOW);
    
    OldSensorValue = 513;
  }
  else
    OldSensorValue = sensorValue;
  
  if (Serial.available())  {
    String cStr = Serial.readStringUntil('\n');
    
    const char *pBuffer = cStr.c_str();
    if (pBuffer[0] == '+')
      bStartTrigger = true;
    else if (pBuffer[0] == '-')
      bStartTrigger = false;
    else if (pBuffer[0] == 'd')
      T1 = atoi(pBuffer + 1);
    else if (pBuffer[0] == 'u')
      T2 = atoi(pBuffer + 1);
  }
}





