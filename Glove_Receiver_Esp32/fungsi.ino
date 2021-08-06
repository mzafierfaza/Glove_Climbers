
int cekRssi(int board) {
  int bar;
  if (boardsStruct[board].sinyal < 0 && boardsStruct[board].sinyal >= -52) {
    bar = 4;
  } else if (boardsStruct[board].sinyal < -52 && boardsStruct[board].sinyal >= -62) {
    bar = 3;
  } else if (boardsStruct[board].sinyal < -62 && boardsStruct[board].sinyal >= -82) {
    bar = 2;
  } else if (boardsStruct[board].sinyal < -82 && boardsStruct[board].sinyal >= -110) {
    bar = 1;
  } else {
    bar = 0;
  }
  return bar;
}

void checkStatus() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= 1000 && check) {
    previousMillis = currentMillis;
    lastValue1 = currentValue1;
    lastValue2 = currentValue2;
    lastValue3 = currentValue3;

    Serial.printf("sinyal_1 value: %d \n", boardsStruct[0].sinyal);
    Serial.printf("stats_1 value: %d \n", lastValue1);
    Serial.printf("lat value: %d \n", boardsStruct[0].latitude_slave);
    Serial.printf("lng value: %d \n", boardsStruct[0].longitude_slave);
    Serial.print("1 CONNECT GAK ? ");
    Serial.println(board1connected);  Serial.println("----------------------");

    Serial.printf("sinyal_2 value: %d \n", boardsStruct[1].sinyal);
    Serial.printf("stats_2 value: %d \n", lastValue2);
        Serial.printf("lat value: %d \n", boardsStruct[1].latitude_slave);
    Serial.printf("lng value: %d \n", boardsStruct[1].longitude_slave);
    Serial.print("2 CONNECT GAK ? ");
    Serial.println(board2connected);  Serial.println("----------------------");

    check = false;
  }
}

void checkConnected() {
  unsigned long currentMillis2 = millis();
  String nilaiSinyal = String(boardsStruct[1].sinyal);
  if (currentMillis2 - previousMillis2 >= 5000) {
    previousMillis2 = currentMillis2;
    //    appendFile(SD, "/RobotikID.txt", nilaiSinyal);
    (lastValue1 != currentValue1) ? board1connected = true : board1connected = false;
    (lastValue2 != currentValue2) ? board2connected = true : board2connected = false;
    check = true;
  }
}


void boardNotConnected() {
  currentValue1 = boardsStruct[0].stats;
  currentValue2 = boardsStruct[1].stats;
  currentValue3 = boardsStruct[2].stats;
  if (!board1connected) boardsStruct[0].sinyal = 0;
  if (!board2connected) boardsStruct[1].sinyal = 0;
}

void bacaGps() {
  if (gps.encode(SerialGPS.read())) {
    if (gps.location.isValid()) {
      gps_cek = true;
      latitude = gps.location.lat();
      lat_str = String(latitude , 6);
      longitude = gps.location.lng();
      lng_str = String(longitude , 6);
      Serial.print("Latitude = ");
      Serial.println(lat_str);
      Serial.print("Longitude = ");
      Serial.println(lng_str);
    }
    else {
      gps_cek = false;
    }
  }
}
