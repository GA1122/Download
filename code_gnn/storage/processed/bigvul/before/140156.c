base::Time CaptivePortalDetector::GetCurrentTime() const {
  if (time_for_testing_.is_null())
    return base::Time::Now();
  else
    return time_for_testing_;
}
