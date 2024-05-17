void MetricsLog::RecordCurrentSessionData(
    DelegatingProvider* delegating_provider,
    base::TimeDelta incremental_uptime,
    base::TimeDelta uptime) {
  DCHECK(!closed_);
  DCHECK(has_environment_);

  WriteRealtimeStabilityAttributes(incremental_uptime, uptime);

  delegating_provider->ProvideCurrentSessionData(uma_proto());
}
