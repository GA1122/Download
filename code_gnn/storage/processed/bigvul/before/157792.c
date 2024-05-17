bool WebContentsImpl::HasRecentInteractiveInputEvent() const {
  static constexpr base::TimeDelta kMaxInterval =
      base::TimeDelta::FromSeconds(5);
  base::TimeDelta delta =
      ui::EventTimeForNow() - last_interactive_input_event_time_;
  UMA_HISTOGRAM_TIMES("Tabs.TimeSinceLastInteraction", delta);
  return delta <= kMaxInterval;
}
