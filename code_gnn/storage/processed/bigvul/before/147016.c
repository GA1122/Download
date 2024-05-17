bool WebLocalFrameImpl::IsNavigationScheduledWithin(
    double interval_in_seconds) const {
  return GetFrame() &&
         GetFrame()->GetNavigationScheduler().IsNavigationScheduledWithin(
             interval_in_seconds);
}
