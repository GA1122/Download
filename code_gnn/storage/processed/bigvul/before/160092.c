bool BackendImpl::ShouldReportAgain() {
  if (uma_report_)
    return uma_report_ == 2;

  uma_report_++;
  int64_t last_report = stats_.GetCounter(Stats::LAST_REPORT);
  Time last_time = Time::FromInternalValue(last_report);
  if (!last_report || (Time::Now() - last_time).InDays() >= 7) {
    stats_.SetCounter(Stats::LAST_REPORT, Time::Now().ToInternalValue());
    uma_report_++;
    return true;
  }
  return false;
}
