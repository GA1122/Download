void Performance::clearMeasures(const String& measure_name) {
  if (!user_timing_)
    user_timing_ = UserTiming::Create(*this);
  user_timing_->ClearMeasures(measure_name);
}
