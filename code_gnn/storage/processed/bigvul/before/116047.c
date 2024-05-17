bool InDateRange(double begin, double end) {
  Time start_time = Time::FromDoubleT(begin);
  Time end_time = Time::FromDoubleT(end);
  return start_time < Time::Now() && end_time > Time::Now();
}
