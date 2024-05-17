void TestingAutomationProvider::GetMetricEventDuration(
    const std::string& event_name,
    int* duration_ms) {
  *duration_ms = metric_event_duration_observer_->GetEventDurationMs(
      event_name);
}
