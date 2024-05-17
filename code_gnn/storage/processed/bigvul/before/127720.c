std::string GetCookieExpirationDate(int delta_secs) {
  const char* const kWeekDays[] = {
    "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"
  };
  const char* const kMonthNames[] = {
    "Jan", "Feb", "Mar", "Apr", "May", "Jun",
    "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
  };

  Time::Exploded exploded_time;
  (Time::Now() + TimeDelta::FromSeconds(delta_secs)).UTCExplode(&exploded_time);

  return base::StringPrintf("%s, %d %s %d %.2d:%.2d:%.2d GMT",
                            kWeekDays[exploded_time.day_of_week],
                            exploded_time.day_of_month,
                            kMonthNames[exploded_time.month - 1],
                            exploded_time.year,
                            exploded_time.hour,
                            exploded_time.minute,
                            exploded_time.second);
}
