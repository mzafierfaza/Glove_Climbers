

void statusbar() {
  // -- gps --
  display.drawBitmap(0, 0, (gps_cek) ? yesLocation : noLocation, 16, 16, WHITE);

  // -- micro sd--
  display.drawBitmap(110, 0, (sdcard_cek) ? yesMicrosd : noMicrosd, 16, 16, WHITE);

  // -- jam --
  display.setTextColor(WHITE);
  display.setFont();
  display.setTextSize(2);
  display.setCursor(25, 0);
  display.print(hour);
  display.print(":");
  display.print(minute);
  display.setCursor(85, 0);
  display.print(AMPM);

  display.drawLine(0, 17, 128, 17, WHITE);

}

void homescreen() {
  // ---- icon Sinyal Master ----
  display.drawBitmap(0, 22, (cekRssi(0) == 4) ? signal4bar
                     : (cekRssi(0) == 3) ? signal3bar : (cekRssi(0) == 2) ? signal2bar
                     : (cekRssi(0) == 1) ? signal1bar : (!board1connected) ? signalNot
                     : signalNot, 20, 20, WHITE);
  display.drawBitmap(0, 44, (cekRssi(1) == 4) ? signal4bar
                     : (cekRssi(1) == 3) ? signal3bar : (cekRssi(1) == 2) ? signal2bar
                     : (cekRssi(1) == 1) ? signal1bar : (!board2connected) ? signalNot
                     : signalNot, 20, 20, WHITE);
  display.setTextColor(WHITE);
  display.setFont(&FreeSans9pt7b);
  display.setTextSize(1);

  // ---- RSSI Sinyal Master ----
  display.setCursor(20, 37);
  display.print((board1connected) ? String(boardsStruct[0].sinyal) : " - ");
  display.setCursor(20, 57);
  display.print((board2connected) ? String(boardsStruct[1].sinyal) : " - ");

  display.drawLine(75, 18, 75, 64, WHITE);

  // ---- Pressure ----
  display.setTextColor(WHITE);
  display.setFont();
  display.setTextSize(1);
  display.setCursor(80, 20);
  display.print((bmp_cek) ? String(pressure) : "-");
  display.setTextSize(2);
  display.setCursor(90, 30);
  display.print("hPa");
  display.setTextSize(1);

  // ---- Temp ----
  display.setCursor(80, 50);
  display.print((bmp_cek) ? String(temp) : "-");
  display.setTextSize(2);
  display.setCursor(104, 50);
  display.print("C");
  display.drawCircle(100, 50, 2, WHITE);
}


void subscreen() {

  // ---- Lat,Long ----
  display.drawBitmap(0, 20, marker, 16, 16, WHITE);
  display.setTextColor(WHITE);
  display.setFont();
  display.setTextSize(1);
  display.setCursor(20, 21);
  display.print("Lat :");
  display.print((gps_cek) ? lat_str : "0");
  display.setCursor(20, 31);
  display.print("Long :");
  display.print((gps_cek) ? lng_str : "0");

  // ---- Counter Store ----
  display.setCursor(2, 43);
  display.print("Ready to store");
  display.setCursor(2, 53);
  display.print("datalogger in");
  display.setTextSize(2);
  display.setCursor(93, 46);
  display.print(counting);
  display.setCursor(118, 50);
  display.setTextSize(1);
  display.print("s");
}
