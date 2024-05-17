ResourceLoadTiming* PerformanceNavigationTiming::GetResourceLoadTiming() const {
  return resource_timing_info_->FinalResponse().GetResourceLoadTiming();
}
