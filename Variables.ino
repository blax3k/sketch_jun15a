
/*
   populate the variable array
*/
void fillVarArray()
{
  for (int i = 0; i < VAR_COUNT; i++) {
    getVar(i);
  }
}

/*
   refresh a specific variable in the variable String Array
*/
void getVar(int i) {
  switch (i) {
    case 0: variables[0] = String(stateName) + "~" + "int" + "~" + state;
      break;
    case 1: variables[1] = String(serialNumberName) + '~' + "string" + '~' + serialNumber;
      break;
    case 2: variables[2] = String(firmNumberName) + '~' + "string" + '~' + firmNumber;
      break;
    case 3: variables[3] = String(boolVarName) + '~' + "bool" + '~' + boolVar;
      break;
    case 4: variables[4] = String(charVarName) + '~' + "char" + '~' + charVar;
      break;
    case 5: variables[5] = String(longVarName) + '~' + "long" + '~' + longVar;
      break;
    case 6: variables[6] = String(ulongVarName) + '~' + "ulong" + '~' + ulongVar;
      break;
    case 7: variables[7] = String(wordVarName) +  '~' + "word" + '~' + wordVar;
      break;
    case 8: variables [8] = String(floatVarName) + '~' + "float" + '~' +  floatToStr(floatVar);
      break;
      case 9: variables[9] = String(doubleVarName) + '~' + "double" + '~' + doubleToStr(doubleVar);
  }
}

void setVar(int i, String newValue)
{
  switch (i) {
    case 0: state = newValue.toInt();
      break;
    case 1: serialNumber = newValue;
      Serial.println(serialNumber);
      break;
    case 2: firmNumber = newValue;
      break;
    case 3: boolVar = toBool(newValue);
      break;
    case 4: charVar = toChar(newValue);
      break;
    case 5: longVar = toLong(newValue);
      break;
    case 6: ulongVar = toULong(newValue);
      break;
    case 7: wordVar = toWord(newValue);
      break;
    case 8: floatVar = newValue.toFloat();
      break;
      case 9: doubleVar = newValue.toFloat();
      break;
  }
  getVar(i); //update the variable list
}

char toChar(String string) {
  char nvCharArray[2];
  string.toCharArray(nvCharArray, 2);
  return nvCharArray[0];
}

boolean toBool(String string) {
  if (string.equals("1"))
    return true;
  else if (string.equals("0"))
    return false;
}

word toWord(String string) {
  char nv[11] ;
  string.toCharArray(nv, 11);
  return strtoul(nv, NULL, 0);
}

long toLong(String string) {
  char nv[11] ;
  string.toCharArray(nv, 11);
  return atol(nv);
}

word toULong(String string) {
  char nv[11] ;
  string.toCharArray(nv, 11);
  return strtoul(nv, NULL, 0);
}

word toDouble(String string){
  char nv[20] = "";
  string.toCharArray(nv, 11);
  return strtoul(nv, NULL, 0);
  
}
String floatToStr(float fl){
  char result[20] = "";
  dtostrf(fl, 3, 5, result); 
  String rValue(result);
  return rValue;
}

String doubleToStr(double fl){
  char result[20] = "";
  dtostrf(fl, 3, 5, result); 
  String rValue(result);
  return rValue;
  
}

void printVariable(int i, String var) {
  Serial.write("VAR=");
  Serial.print(i);
  Serial.write("~");
  Serial.print(var);
  Serial.println();
}
