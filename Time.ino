/*
   Prints the date and time to the serial output
*/
void digitalClockDisplay() {
  // digital clock display of the time
  Serial.write("time= ");
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  Serial.print("  day: ");
  Serial.print(day());
  Serial.print("  month: ");
  Serial.print(month());
  Serial.print("  year: ");
  Serial.print(year());
  Serial.println();
}


void processSyncMessage(String timeString) {
  // if time sync available from serial port, update time and return true
  //  String timeString = Serial.readString();
  char c = timeString[0];
  //  if ( c == TIME_HEADER ) {
  time_t pctime = 0;
  for (int i = 8; i < TIME_MSG_LEN - 1; i++) {
    c = timeString[i];
    if ( c >= '0' && c <= '9') {
      pctime = (10 * pctime) + (c - '0') ; // convert digits to a number
    }
  }
  setTime(pctime); // Sync Arduino clock to the time received on the serial port
}


void printDigits(int digits) {
  // utility function for digital clock display: prints preceding colon and leading 0
  Serial.print(":");
  if (digits < 10)
    Serial.print('0');
  Serial.print(digits);
}

