FPDF_SYSTEMTIME PDFiumEngine::Form_GetLocalTime(FPDF_FORMFILLINFO* param) {
  base::Time time = base::Time::Now();
  base::Time::Exploded exploded;
  time.LocalExplode(&exploded);

  FPDF_SYSTEMTIME rv;
  rv.wYear = exploded.year;
  rv.wMonth = exploded.month;
  rv.wDayOfWeek = exploded.day_of_week;
  rv.wDay = exploded.day_of_month;
  rv.wHour = exploded.hour;
  rv.wMinute = exploded.minute;
  rv.wSecond = exploded.second;
  rv.wMilliseconds = exploded.millisecond;
  return rv;
}
