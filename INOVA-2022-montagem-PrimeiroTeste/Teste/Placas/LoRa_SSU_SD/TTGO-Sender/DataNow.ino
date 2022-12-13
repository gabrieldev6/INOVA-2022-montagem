void dateNow(){

  rtc.getDate(year, month, day, weekday);
  rtc.getTime(hour, minute, second, period);
  if (!(second % 3)) rtc.setMode(1 - rtc.getMode());
  rtc.getTime(hour, minute, second, period);
  
  // Data
  timestemp="";
  
  timestemp+=day;
  timestemp+="/";
  timestemp+=month;
  timestemp+="/";
  timestemp+=year + 2000;
  timestemp+=";";
  timestemp+=hour;
  timestemp+=":";
  timestemp+=minute;
  timestemp+=":";
  timestemp+=second;
  //timestemp+=rtc.getMode() ? (period ? "PM" :" AM") : "";
}