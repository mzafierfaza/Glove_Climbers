

void storeDataToSD() {
  unsigned long currentMillis3 = millis();
  if (currentMillis3 - previousMillis3 >= 1000) {
    previousMillis3 = currentMillis3;
    counting--;
    sekaliKirim = true;
  }
  if (counting == 0 && sekaliKirim) {
    logSDCard();
    sekaliKirim = false;
  }
  if (counting < 0) counting = 60;
}

void everySDStart() {

  // If the data.txt file doesn't exist
  // Create a file on the SD card and write the data labels
  File file = SD.open("/DataLogger.txt");
  if (!file) {
    Serial.println("File doens't exist");
    Serial.println("Creating file...");
    writeFile(SD, "/DataLogger.txt", "------------------- DataLogger Pendaki Gunung ----------------- \r\n");
  }
  else {
    Serial.println("File already exists");
    appendFile(SD, "/DataLogger.txt", "\r\n\n ------------------- DataLogger Pendaki Gunung ----------------- \r\n");
  }
  file.close();
}


void logSDCard() {

  //"Day,   Date,   Month,   Year,   Hour,   Minute,    Member 1,    Member 2,    Pressure,   Temperature,    Latitude,    Longitude  \r\n";
  dataMessage = String(days[weekday]) + ", " + String(monthday) + " " + String(months[month]) + " 20" + String(year) + ", " + String(hour) + ":" + String(minute) + String(AMPM) + " | " + 
                "1 = " + String(boardsStruct[0].sinyal) + ", 2 = " + String(boardsStruct[1].sinyal) + (" | ") +
                "Suhu = " + String(temp) + "`C, Pressure = " + String(pressure) + " hPa, | " +
                "Latitude = " + lat_str + ", Longitude = " + lng_str + "\r\n";
  Serial.print("Save data: ");
  Serial.println(dataMessage);
  appendFile(SD, "/DataLogger.txt", dataMessage.c_str());
}

// Write to the SD card (DON'T MODIFY THIS FUNCTION)
void writeFile(fs::FS &fs, const char * path, const char * message) {
  Serial.printf("Writing file: %s\n", path);

  File file = fs.open(path, FILE_WRITE);
  if (!file) {
    Serial.println("Failed to open file for writing");
    return;
  }
  if (file.print(message)) {
    Serial.println("File written");
  } else {
    Serial.println("Write failed");
  }
  file.close();
}


// Append data to the SD card (DON'T MODIFY THIS FUNCTION)
void appendFile(fs::FS &fs, const char * path, const char * message) {
  Serial.printf("Appending to file: %s\n", path);

  File file = fs.open(path, FILE_APPEND);
  if (!file) {
    Serial.println("Failed to open file for appending");
    return;
  }
  if (file.print(message)) {
    Serial.println("Message appended");
  } else {
    Serial.println("Append failed");
  }
  file.close();
}

void membacaWaktu() {

  Wire.beginTransmission(I2C_ADDRESS);
  Wire.write(byte(0));
  Wire.endTransmission();
  Wire.requestFrom(I2C_ADDRESS, 7);
  second = bcdToDec(Wire.read());
  minute = bcdToDec(Wire.read());
  hour = bcdToDec(Wire.read());
  weekday = bcdToDec(Wire.read());
  monthday = bcdToDec(Wire.read());
  month = bcdToDec(Wire.read());
  year = bcdToDec(Wire.read());
  if (hour > 12) {
    hour -= 12;
    AMPM = "PM";
  }
  else AMPM = "AM";
  //  Serial.print(days[weekday]);
  //  Serial.print(" ");
  //  Serial.print(monthday);
  //  Serial.print(" ");
  //  Serial.print(months[month]);
  //  Serial.print(" 20");
  //  Serial.print(year);
  //  Serial.print(" ");
  //  Serial.print(hour);
  //  Serial.print(":");
  //  sprintf(buffer, "%02d", minute);
  //  Serial.print(buffer);
  //  Serial.print(":");
  //  sprintf(buffer, "%02d", second);
  //  Serial.println(buffer);


}

void mengaturWaktu() {

  Wire.beginTransmission(I2C_ADDRESS);
  Wire.write(byte(0));
  Wire.write(decToBcd(58)); // second
  Wire.write(decToBcd(setMenit)); // minute
  Wire.write(decToBcd(setJam)); // hour
  Wire.write(decToBcd(setHari));  // weekday
  Wire.write(decToBcd(setTanggal)); // date
  Wire.write(decToBcd(setBulan));  // month
  Wire.write(decToBcd(setTahun)); // year
  Wire.write(byte(0));
  Wire.endTransmission();
}

byte decToBcd(byte val) {
  return ((val / 10 * 16) + (val % 10));
}
byte bcdToDec(byte val) {
  return ((val / 16 * 10) + (val % 16));
}
