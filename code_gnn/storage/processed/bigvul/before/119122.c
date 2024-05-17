int GetLastYear() {
  Time last_year_time = Time::NowFromSystemTime() - TimeDelta::FromDays(365);
  Time::Exploded exploded;
  last_year_time.LocalExplode(&exploded);
  return exploded.year;
}
