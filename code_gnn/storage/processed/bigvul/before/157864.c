void WebContentsImpl::SetTopControlsGestureScrollInProgress(bool in_progress) {
  if (delegate_)
    delegate_->SetTopControlsGestureScrollInProgress(in_progress);
}
