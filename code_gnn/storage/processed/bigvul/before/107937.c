void InfoBar::DestroyFocusTracker(bool restore_focus) {
  if (focus_tracker_.get()) {
    if (restore_focus)
      focus_tracker_->FocusLastFocusedExternalView();
    focus_tracker_->SetFocusManager(NULL);
    focus_tracker_.reset(NULL);
  }
}
