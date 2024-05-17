void Instance::DocumentLoadFailed() {
  DCHECK(document_load_state_ == LOAD_STATE_LOADING);
  UserMetricsRecordAction("PDF.LoadFailure");

  progress_bar_.Fade(false, kProgressFadeTimeoutMs);

  if (did_call_start_loading_) {
    pp::PDF::DidStopLoading(this);
    did_call_start_loading_ = false;
  }

  document_load_state_ = LOAD_STATE_FAILED;
  paint_manager_.InvalidateRect(pp::Rect(pp::Point(), plugin_size_));
}
