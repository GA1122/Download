void RenderViewImpl::SetReportLoadProgressEnabled(bool enabled) {
  if (!enabled) {
    load_progress_tracker_.reset(NULL);
    return;
  }
  if (load_progress_tracker_ == NULL)
    load_progress_tracker_.reset(new LoadProgressTracker(this));
}
