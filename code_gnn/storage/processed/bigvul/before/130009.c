void VariationsRequestSchedulerMobile::OnAppEnterForeground() {
  if (base::Time::Now() <
      last_request_time_ + base::TimeDelta::FromHours(kSeedFetchPeriodHours)) {
    return;
  }

  schedule_fetch_timer_.Start(
      FROM_HERE,
      base::TimeDelta::FromSeconds(kScheduleFetchDelaySeconds),
      this,
      &VariationsRequestSchedulerMobile::Start);
}
