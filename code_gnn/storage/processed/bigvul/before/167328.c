base::Time AnHourAgo() {
  return base::Time::Now() - base::TimeDelta::FromHours(1);
}
