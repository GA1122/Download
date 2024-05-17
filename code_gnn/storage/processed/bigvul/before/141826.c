void MetricsLog::RecordPreviousSessionData(
    DelegatingProvider* delegating_provider) {
  delegating_provider->ProvidePreviousSessionData(uma_proto());
}
