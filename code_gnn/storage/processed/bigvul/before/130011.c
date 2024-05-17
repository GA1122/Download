void VariationsRequestSchedulerMobile::Start() {
  const base::Time last_fetch_time = base::Time::FromInternalValue(
      local_state_->GetInt64(prefs::kVariationsLastFetchTime));
  if (base::Time::Now() >
      last_fetch_time + base::TimeDelta::FromHours(kSeedFetchPeriodHours)) {
    last_request_time_ = base::Time::Now();
    task().Run();
  }
}
