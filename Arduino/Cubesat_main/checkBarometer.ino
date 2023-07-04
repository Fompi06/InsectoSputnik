void checkBarometer(void) {
  barometer.checkUpdates();
  if (barometer.isReady()) {
    double pressure;
    double temperature;
    temperature = barometer.GetTemp();  // Returns temperature in C
    pressure = barometer.GetPres();     // Returns pressure in Pascals

    delay(250);
    DEBUG("tBar:  ");
    DEBUGLN(temperature);
    DEBUG("pres: ");
    DEBUGLN(pressure);
    pres = pressure;
    if ((temperature != prevTemperature) || (pressure != prevPressure)) {

      // Calculate predicted seaLevel pressure based off a known altitude in meters
      if (seaLevelPressure == 0) seaLevelPressure = barometer.getSeaLevel(217.3);



      // Calculate current altitude based on pressure reading
      alt = barometer.getAltitude();
      if (altStart == 0)
      {
        altStart = alt;
      }
        
      altRel = alt - altStart;

      DEBUG("alt: ");
      DEBUG(alt);
      DEBUGLN(" meters");
      DEBUG("altRel: ");
      DEBUG(altRel);
      DEBUGLN(" meters");

      prevTemperature = temperature;
      prevPressure = pressure;
      loraSend("1,");
      loraSend(temperature);
      loraSend(",");
      loraSend(pressure);
      loraSend(",");
      loraSend(alt);
      loraSend(",");
      loraSendln(altRel);
      log("1,");
      log(millis());
      log(",");
      logTime();
      log(",");
      log(temperature);
      log(",");
      log(pressure);
      log(",");
      log(alt);
      log(",");
      logln(altRel);
    }
  }
}
// void checkBarometer() {
//   double pressure = 0;
// 	double temperature = 0;
// 	double altitude = 0;
// barometer.checkUpdates();
	   
// 	/* 
// 	Updates Temperature and Pressure values for reading.  Until sensor is done with first set of calculations values will be zero.
// 	At max oversampling from the time the sensor is first connected to the time all conversions are complete is
// 	3 ms for booting + 10 ms for each step = ~43 ms + the amount of time it takes to run through a loop.
	
// 	Alternatively, prior to reading temperature and pressure data, check isReady().
// 	*/
// 	if (barometer.isReady()) { 
		
// 		temperature = barometer.GetTemp(); // Returns temperature in C
// 		pressure = barometer.GetPres(); // Returns pressure in Pascals
		
// 		// If a new temperature or pressure reading comes in then the sensor has been polled.
// 		// Print the new values and the time, in ms, between readings
// 		if ((temperature != prevTemperature) || (pressure != prevPressure)) {
			
// 			// Calculate predicted seaLevel pressure based off a known altitude in meters
// 			if (seaLevelPressure == 0) seaLevelPressure = barometer.getSeaLevel(217.3); 

// 			Serial.print(F("The current pressure is: "));
// 			Serial.print(pressure);
// 			Serial.println(F(" Pascals"));

// 			Serial.print(F("The current temperature is: "));
// 			Serial.print(temperature);
// 			Serial.println(F("°C"));
		
// 			Serial.print(F("The calculated pressure at seaLevel is: "));
// 			Serial.print(seaLevelPressure);
// 			Serial.println(F(" Pascals"));
		
// 			// Calculate current altitude based on pressure reading
// 			altitude = barometer.getAltitude();
// 			Serial.print(F("The calculated altitude is: "));
// 			Serial.print(altitude);
// 			Serial.println(F(" meters"));
			
// 			// Calculate current altitude performing temperature corrections
// 			altitude = barometer.getAltitude(true);
// 			Serial.print(F("The calculated altitude with temperature correction is: "));
// 			Serial.print(altitude);
// 			Serial.println(F(" meters"));
			
// 			Serial.println();
// 			Serial.println();

// 			prevTemperature = temperature;
// 			prevPressure = pressure;
// 		}
// 	}
//   }