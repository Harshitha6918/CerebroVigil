#define BLYNK_TEMPLATE_ID "TMPL3dDiwqUHU"
#define BLYNK_TEMPLATE_NAME "ecg"
#define BLYNK_AUTH_TOKEN "xKZJoitG3plGKDIEsemTFvzphn7TVY5Y"
#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

const char ssid[] = "Calm";
const char password[] = "triveni@08";

int sensor = 32; 
int lo1 = 4; 
int lo2 = 2; 

void setup() { 
  Serial.begin(115200);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, password);
  
  pinMode(sensor, INPUT); 
  pinMode(lo1, INPUT); 
  pinMode(lo2, INPUT); 
}

void loop() { 
  static unsigned long startTime = millis();
  static int peakValue = 0; 
  
  // Check for inputs from lo1 and lo2
  if ((digitalRead(lo1) == 1) || (digitalRead(lo2) == 1)) { 
    Serial.println('!'); 
  } else { 
    // Read analog value from ECG sensor
    int ecgValue = analogRead(sensor);
    
    // Update peak value if current reading is higher
    if (ecgValue > peakValue) {
      peakValue = ecgValue;
    }
    
    // Print the current ECG value
    Serial.println(ecgValue); 
    
    // Send ECG value to Blynk (virtual pin V1)
    Blynk.virtualWrite(V1, ecgValue);
    
    // Log event if ECG value exceeds 3000
    if (ecgValue > 3000) {
      Blynk.logEvent("tumor_event", "Brain tumor symptoms detected, need to consult doctor");  // Change "ecg_alert" to your desired event name
    }
  } 
  
  // Check if one minute has passed
  if (millis() - startTime >= 60000) {
    Serial.print("Peak ECG value in one minute: ");
    Serial.println(peakValue);
    
    // Send peak value to Blynk (virtual pin V2)
    Blynk.virtualWrite(V2, peakValue);
    
    // Reset for next minute
    peakValue = 0;  
    startTime = millis(); // Reset the timer
  }
  
  // Run Blynk
  Blynk.run();
  
  // Wait for a bit to keep serial data from saturating
  delay(1); 
}
