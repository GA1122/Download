void BrowserView::OnWindowEndUserBoundsChange() {
  if (!interactive_resize_)
    return;
  auto now = base::TimeTicks::Now();
  DCHECK(!interactive_resize_->begin_timestamp.is_null());
  UMA_HISTOGRAM_TIMES("BrowserWindow.Resize.Duration",
                      now - interactive_resize_->begin_timestamp);
  UMA_HISTOGRAM_COUNTS_1000("BrowserWindow.Resize.StepCount",
                            interactive_resize_->step_count);
  interactive_resize_.reset();
}
