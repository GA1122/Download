base::Time NavigationControllerImpl::TimeSmoother::GetSmoothedTime(
    base::Time t) {
  if (low_water_mark_ <= t && t <= high_water_mark_) {
    high_water_mark_ += base::TimeDelta::FromMicroseconds(1);
    return high_water_mark_;
  }

  low_water_mark_ = high_water_mark_ = t;
  return t;
}
