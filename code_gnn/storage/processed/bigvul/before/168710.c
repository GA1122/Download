unsigned long long PerformanceNavigationTiming::GetEncodedBodySize() const {
  return resource_timing_info_->FinalResponse().EncodedBodyLength();
}
