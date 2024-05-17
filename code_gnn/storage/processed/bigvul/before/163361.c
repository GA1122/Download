bool RenderThreadImpl::NeedsToRecordFirstActivePaint(
    int ttfap_metric_type) const {
  if (ttfap_metric_type == RenderWidget::TTFAP_AFTER_PURGED)
    return needs_to_record_first_active_paint_;

  if (was_backgrounded_time_.is_min())
    return false;
  base::TimeDelta passed = base::TimeTicks::Now() - was_backgrounded_time_;
  return passed.InMinutes() >= 5;
}
