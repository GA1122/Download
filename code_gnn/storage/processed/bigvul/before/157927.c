void RenderViewImpl::FrameDidStopLoading(WebFrame* frame) {
  if (frames_in_progress_ == 0)
    return;
  frames_in_progress_--;
  if (frames_in_progress_ == 0) {
    for (auto& observer : observers_)
      observer.DidStopLoading();
  }
}
