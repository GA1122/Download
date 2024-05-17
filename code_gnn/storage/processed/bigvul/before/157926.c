void RenderViewImpl::FrameDidStartLoading(WebFrame* frame) {
  DCHECK_GE(frames_in_progress_, 0);
  if (frames_in_progress_ == 0) {
    for (auto& observer : observers_)
      observer.DidStartLoading();
  }
  frames_in_progress_++;
}
