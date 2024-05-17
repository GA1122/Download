void OutOfProcessInstance::DocumentLoadFailed() {
  DCHECK(document_load_state_ == LOAD_STATE_LOADING);
  UserMetricsRecordAction("PDF.LoadFailure");

  if (did_call_start_loading_) {
    pp::PDF::DidStopLoading(this);
    did_call_start_loading_ = false;
  }

  document_load_state_ = LOAD_STATE_FAILED;
  paint_manager_.InvalidateRect(pp::Rect(pp::Point(), plugin_size_));

  pp::VarDictionary message;
  message.Set(pp::Var(kType), pp::Var(kJSLoadProgressType));
  message.Set(pp::Var(kJSProgressPercentage), pp::Var(-1));
  PostMessage(message);
}
