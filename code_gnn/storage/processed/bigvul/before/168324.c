void BrowserView::OnBoundsChanged(const gfx::Rect& previous_bounds) {
  if (!interactive_resize_)
    return;
  auto now = base::TimeTicks::Now();
  if (!interactive_resize_->last_resize_timestamp.is_null()) {
    const auto& current_size = size();
    if (current_size == previous_bounds.size())
      return;
    UMA_HISTOGRAM_CUSTOM_TIMES("BrowserWindow.Resize.StepInterval",
                               now - interactive_resize_->last_resize_timestamp,
                               base::TimeDelta::FromMilliseconds(1),
                               base::TimeDelta::FromSeconds(1), 50);
    UMA_HISTOGRAM_CUSTOM_COUNTS(
        "BrowserWindow.Resize.StepBoundsChange.Width",
        std::abs(previous_bounds.size().width() - current_size.width()),
        1  , 300  , 100  );
    UMA_HISTOGRAM_CUSTOM_COUNTS(
        "BrowserWindow.Resize.StepBoundsChange.Height",
        std::abs(previous_bounds.size().height() - current_size.height()),
        1  , 300  , 100  );
  }
  ++interactive_resize_->step_count;
  interactive_resize_->last_resize_timestamp = now;
}
