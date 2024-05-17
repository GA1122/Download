AtomicString PerformanceNavigationTiming::ConnectionInfo() const {
  return resource_timing_info_->FinalResponse().ConnectionInfoString();
}
