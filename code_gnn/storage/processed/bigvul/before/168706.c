unsigned long long PerformanceNavigationTiming::GetDecodedBodySize() const {
  return resource_timing_info_->FinalResponse().DecodedBodyLength();
}
