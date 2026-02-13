hw_timer_t *timer0 = NULL;
volatile bool ledState = false;
volatile unsigned long blinkInterval = 1000000; 
bool timerEnabled = false;

void IRAM_ATTR onTimer() {
  ledState = !ledState;
  digitalWrite(RGB_BUILTIN, ledState);
}
void setup() {
  Serial.begin(115200);
  pinMode(RGB_BUILTIN, OUTPUT);
  
  timer0 = timerBegin(1000000); 
  timerAttachInterrupt(timer0, &onTimer);
  timerAlarm(timer0, blinkInterval, true, 0);
  
  timerStop(timer0);
  
  Serial.println("LED Blink Controller Ready");
  Serial.println("Send 'F' for fast blinking (0.5s)");
  Serial.println("Send 'S' for slow blinking (2s)");
  Serial.println("Send 'X' to stop blinking");
}
void loop() {
  if (Serial.available() > 0) {
    char command = Serial.read();
    
    switch(command) {
      case 'F':
      case 'f':
        setBlinkSpeed(500000); 
        Serial.println("Fast blinking activated (0.5s interval)");
        break;
        
      case 'S':
      case 's':
        setBlinkSpeed(2000000); 
        Serial.println("Slow blinking activated (2s interval)");
        break;
        
      case 'X':
      case 'x':
        stopBlinking();
        Serial.println("Blinking stopped");
        break;
        
      default:
        Serial.println("Invalid command. Use F (fast), S (slow), or X (stop)");
        break;
    }
    
    while(Serial.available() > 0) {
      Serial.read();
    }
  }
}
void setBlinkSpeed(unsigned long interval) {
  if (timerEnabled) {
    timerStop(timer0);
  }
  
  
  blinkInterval = interval;
  

  timerAlarmWrite(timer0, blinkInterval, true);
  timerStart(timer0);
  
  timerEnabled = true;
}

void stopBlinking() {
  if (timerEnabled) {
    timerStop(timer0);
    timerEnabled = false;
    digitalWrite(RGB_BUILTIN, LOW);
    ledState = false;
  }
}

void timerAlarmWrite(hw_timer_t *timer, uint64_t alarm_value, bool autoreload) {
  timerWrite(timer, 0); 
  timerAlarm(timer, alarm_value, autoreload, 0);
}