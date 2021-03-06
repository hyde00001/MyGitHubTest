//Put uBlox GPS into Flight Mode - essential for getting a GPS Fix >10K M.
//Put uBlox GPS into MaxPower Mode - best for establishing GPS Fix 
//Don't proceed until achieved. If we cant' get it done, coldstart the GPS and keep trying
void mySetFlightAndPowerOnRestart()
{
  int myThreshold = 0;
  do
  {  
    if (myThreshold > myGPSFlightModeAttemptsBeforeColdStart)
    {
      myGPSColdStart ();
      myThreshold = 0;
    }
    
    if (!myGPSFlightModeYesNo)
    {
      myGPSFlightMode();
    }
    
    if (!myGPSMaxPowerMode)
    {
      myGPSMaxPowerMode();
    }
    
    myThreshold ++;
  }
  while (!myGPSFlightModeYesNo || !myGPSMaxPowerModeYesNo);
}


void myGPSHotStart()
{
  unsigned long myTimer;
  
  Serial1.begin(9600);
  
  byte gps_set_sucess = 0;
  if (myDebugShowGPSStatus) {
    Serial.print(F("GPS HotStart: "));
  }
  uint8_t GPSHotStart[] = {0xB5, 0x62, 0x06, 0x04, 0x04, 0x00, 0x00, 0x00, 0x01, 0x00, 0x0F, 0x66                   }; 
  myTimer = millis(); 
  while(!gps_set_sucess && (millis() - myTimer < 2000))
  {
    sendUBX(GPSHotStart, sizeof(GPSHotStart)/sizeof(uint8_t));
    //gps_set_sucess=getUBX_ACK(GPSHotStart);
  }
  
  Serial1.end();
  myGPSHotStartCount++;
  myGPSPowerSaveModeYesNo = false;
  myGPSFlightModeYesNo = false;
}


void myGPSWarmStart()
{
  unsigned long myTimer;
  
  Serial1.begin(9600);

  byte gps_set_sucess = 0;
  if (myDebugShowGPSStatus) {
    Serial.print(F("GPS WarmStart: "));
  }
  uint8_t GPSHotStart[] = {0xB5, 0x62, 0x06, 0x04, 0x04, 0x00, 0x01, 0x00, 0x01, 0x00, 0x10, 0x6A                   }; 
  myTimer = millis(); 
  while(!gps_set_sucess && (millis() - myTimer < 2000))
  {
    sendUBX(GPSHotStart, sizeof(GPSHotStart)/sizeof(uint8_t));
    //gps_set_sucess=getUBX_ACK(GPSHotStart);
  }
  
  Serial1.end();
  myGPSWarmStartCount++;
  myGPSPowerSaveModeYesNo = false;
  myGPSFlightModeYesNo = false;
}



void myGPSColdStart()
{
  unsigned long myTimer;
  
  Serial1.begin(9600);
  
  byte gps_set_sucess = 0;
  if (myDebugShowGPSStatus) {
    Serial.print(F("GPS ColdStart: "));
  }
  uint8_t GPSColdStart[] = {0xB5, 0x62, 0x06, 0x04, 0x04, 0x00, 0xFF, 0x87, 0x01, 0x00, 0x95, 0xF7                   }; 
  myTimer = millis(); 
  while(!gps_set_sucess && (millis() - myTimer < 2000))
  {
    sendUBX(GPSColdStart, sizeof(GPSColdStart)/sizeof(uint8_t));
    //gps_set_sucess=getUBX_ACK(GPSColdStart);
  }
  
  Serial1.end();
  myGPSColdStartCount++;
  myGPSPowerSaveModeYesNo = false;
  myGPSFlightModeYesNo = false;
}


void myGPSPowerSaveMode()
{
  unsigned long myTimer;
  
  Serial1.begin(9600);
  
  byte gps_set_sucess = 0;
  if (myDebugShowGPSStatus) {
    Serial.print(F("GPS PowerSaveMode: "));
  }
  uint8_t GPSPowerSaveMode[] = {0xB5, 0x62, 0x06, 0x11, 0x02, 0x00, 0x08, 0x01, 0x22, 0x92                   }; 
  myTimer = millis(); 
  while(!gps_set_sucess && (millis() - myTimer < 2000))
  {
    sendUBX(GPSPowerSaveMode, sizeof(GPSPowerSaveMode)/sizeof(uint8_t));
    gps_set_sucess=getUBX_ACK(GPSPowerSaveMode);
  }
  
  Serial1.end();
  
  if(gps_set_sucess)
  {
    myGPSPowerSaveModeYesNo = true;
    myGPSMaxPowerModeYesNo = false;
  }
}

void myGPSMaxPowerMode()
{
  unsigned long myTimer;
  
  Serial1.begin(9600);

  byte gps_set_sucess = 0;
  if (myDebugShowGPSStatus) {
    Serial.print(F("GPS MaxPowerMode: "));
  }
  uint8_t GPSMaxPowerMode[] = {0xB5, 0x62, 0x06, 0x11, 0x02, 0x00, 0x08, 0x00, 0x21, 0x91                   }; 
  myTimer = millis(); 
  while(!gps_set_sucess && (millis() - myTimer < 2000))
  {
    sendUBX(GPSMaxPowerMode, sizeof(GPSMaxPowerMode)/sizeof(uint8_t));
    gps_set_sucess=getUBX_ACK(GPSMaxPowerMode);
  }
  
  Serial1.end();
  
  if(gps_set_sucess)
  {
    myGPSMaxPowerModeYesNo = true;
    myGPSPowerSaveModeYesNo = false;
  }
}


void myGPSFlightMode()
{
  unsigned long myTimer;
  
  Serial1.begin(9600);
  /*
  B5 62 06 24 24 00 FF FF 06 03 00 00 00 00 10 27 00 00 05 00 FA 00 FA
  00 64 00 2C 01 00 00 00 00 10 27 00 00 00 00 00 00 00 00 4D DB
  */
  byte gps_set_sucess = 0;
  if (myDebugShowGPSStatus) {
    Serial.print(F("GPS Airborne 1 Mode: "));
  }
  uint8_t GPSAirborne1Mode[] = {0xB5, 0x62, 0x06, 0x24, 0x24, 0x00, 0xFF, 0xFF, 0x06, 0x03, 0x00, 0x00, 0x00, 0x00, 0x10, 0x27, 0x00, 0x00, 0x05, 0x00, 0xFA, 0x00, 0xFA,
                                0x00, 0x64, 0x00, 0x2C, 0x01, 0x00, 0x00, 0x00, 0x00, 0x10, 0x27, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x4D, 0xDB  }; 
                                
  //PITS                        0xB5, 0x62, 0x06, 0x24, 0x24, 0x00, 0xFF, 0xFF, 0x06, 0x03, 0x00, 0x00, 0x00, 0x00, 0x10, 0x27, 0x00, 0x00, 0x05, 0x00, 0xFA, 0x00, 0xFA, 
  //                            0x00, 0x64, 0x00, 0x2C, 0x01, 0x00, 0x00, 0x00, 0x00,*0x00, 0x00,*0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,*0x16, 0xDC                              
                                
  myTimer = millis(); 
  while(!gps_set_sucess && (millis() - myTimer < 2000))
  {
    sendUBX(GPSAirborne1Mode, sizeof(GPSAirborne1Mode)/sizeof(uint8_t));
    gps_set_sucess=getUBX_ACK(GPSAirborne1Mode);
  }
  
  Serial1.end();
  
  if(gps_set_sucess)
  {
    myGPSFlightModeYesNo = true;
  }
  
}



void myGPSGPGGAOnly()
{
  unsigned long myTimer;
  
  Serial1.begin(9600);
  
  byte gps_set_sucess = 0;
  if (myDebugShowGPSStatus) {
    Serial.print(F("Switching off NMEA GLL: "));
  }
  uint8_t setGLL[] = {0xB5, 0x62, 0x06, 0x01, 0x08, 0x00, 0xF0, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x2B                   }; 
  myTimer = millis(); 
  while(!gps_set_sucess && (millis() - myTimer < 3000))
  {
    sendUBX(setGLL, sizeof(setGLL)/sizeof(uint8_t));
    gps_set_sucess=getUBX_ACK(setGLL);
  }
  gps_set_sucess=0;

 
   if (myDebugShowGPSStatus) {
     Serial.print(F("Switching off NMEA GSA: "));
   }
   uint8_t setGSA[] = { 
   0xB5, 0x62, 0x06, 0x01, 0x08, 0x00, 0xF0, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x32                   };
   myTimer = millis();
   while(!gps_set_sucess && (millis() - myTimer < 3000))
   {	
   sendUBX(setGSA, sizeof(setGSA)/sizeof(uint8_t));
   gps_set_sucess=getUBX_ACK(setGSA);
   }
   gps_set_sucess=0;
   
   if (myDebugShowGPSStatus) {
     Serial.print(F("Switching off NMEA GSV: "));
   }
   uint8_t setGSV[] = { 
   0xB5, 0x62, 0x06, 0x01, 0x08, 0x00, 0xF0, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x39                   };
   myTimer = millis();
   while(!gps_set_sucess && (millis() - myTimer < 3000))
   {
   sendUBX(setGSV, sizeof(setGSV)/sizeof(uint8_t));
   gps_set_sucess=getUBX_ACK(setGSV);
   }
   gps_set_sucess=0;
   
   if (myDebugShowGPSStatus) {
     Serial.print(F("Switching off NMEA RMC: "));
   }
   uint8_t setRMC[] = { 
   0xB5, 0x62, 0x06, 0x01, 0x08, 0x00, 0xF0, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x04, 0x40                   };
   myTimer = millis();
   while(!gps_set_sucess && (millis() - myTimer < 3000))
   {
   sendUBX(setRMC, sizeof(setRMC)/sizeof(uint8_t));
   gps_set_sucess=getUBX_ACK(setRMC);
   }
   gps_set_sucess=0;
   
   if (myDebugShowGPSStatus) {
     Serial.print(F("Switching off NMEA VTG: "));
   }
   uint8_t setVTG[] = { 
   0xB5, 0x62, 0x06, 0x01, 0x08, 0x00, 0xF0, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x46                   };
   myTimer = millis();
   while(!gps_set_sucess && (millis() - myTimer < 3000))
   {
   sendUBX(setVTG, sizeof(setRMC)/sizeof(uint8_t));
   gps_set_sucess=getUBX_ACK(setVTG);
   }
   gps_set_sucess=0;
   
   Serial1.end();
      
   // Increment the myGPSGPGGAOnlyCount global and (if appropriate) report the value
   myGPSGPGGAOnlyCount++;
   
   if (myDebugShowGPSStatus) {
     Serial.print(F("Total Calls To myGPSGPGGAOnly:"));
     Serial.println(myGPSGPGGAOnlyCount);
   
     // Tidy the debugging messages
     Serial.println(F(""));
   }
}

// Send a byte array of UBX protocol to the GPS
void sendUBX(uint8_t *MSG, uint8_t len) {
  for(int i=0; i<len; i++) {
    Serial1.write(MSG[i]);
    
    if (myDebugShowFullMessage) {
      Serial.print(MSG[i], HEX);
    }
  }
  Serial1.println();
}

// Calculate expected UBX ACK packet and parse UBX response from GPS
boolean getUBX_ACK(uint8_t *MSG) {
  uint8_t b;
  uint8_t ackByteID = 0;
  uint8_t ackPacket[10];
  unsigned long startTime = millis();
  if (myDebugShowGPSStatus) {
    Serial.print(F(" * Reading ACK response: "));
  }
 
  // Construct the expected ACK packet    
  ackPacket[0] = 0xB5;	// header
  ackPacket[1] = 0x62;	// header
  ackPacket[2] = 0x05;	// class
  ackPacket[3] = 0x01;	// id
  ackPacket[4] = 0x02;	// length
  ackPacket[5] = 0x00;
  ackPacket[6] = MSG[2];	// ACK class
  ackPacket[7] = MSG[3];	// ACK id
  ackPacket[8] = 0;		// CK_A
  ackPacket[9] = 0;		// CK_B
 
  // Calculate the checksums
  for (uint8_t i=2; i<8; i++) {
    ackPacket[8] = ackPacket[8] + ackPacket[i];
    ackPacket[9] = ackPacket[9] + ackPacket[8];
  }
 
  while (1) {
 
    // Test for success
    if (ackByteID > 9) {
      // All packets in order!
      if (myDebugShowGPSStatus) {
        Serial.println(" (SUCCESS!)");
      }
      return true;
    }
 
    // Timeout if no valid response in 3 seconds
    if (millis() - startTime > 3000) { 
      if (myDebugShowGPSStatus) {
        Serial.println(" (FAILED!)");
      }
      return false;
    }
 
    // Make sure data is available to read
    if (Serial1.available()) {
      b = Serial1.read();
 
      // Check that bytes arrive in sequence as per expected ACK packet
      if (b == ackPacket[ackByteID]) { 
        ackByteID++;
        if (myDebugShowFullMessage) {
          Serial.print(b, HEX);
        }
      } 
      else {
        ackByteID = 0;	// Reset and look again, invalid order
      }
 
    }
  }
}
