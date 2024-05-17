bool PerformanceNavigationTiming::DidReuseConnection() const {
  return resource_timing_info_->FinalResponse().ConnectionReused();
}
