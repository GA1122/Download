base::Time TruncateToUTCDay(const base::Time& time) {
  base::Time::Exploded exploded;
  time.UTCExplode(&exploded);
  exploded.hour = 0;
  exploded.minute = 0;
  exploded.second = 0;
  exploded.millisecond = 0;

  return base::Time::FromUTCExploded(exploded);
}
