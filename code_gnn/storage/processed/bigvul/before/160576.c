void RenderFrameImpl::DidChangeScrollOffset() {
  render_view_->StartNavStateSyncTimerIfNecessary(this);

  {
    SCOPED_UMA_HISTOGRAM_TIMER("RenderFrameObservers.DidChangeScrollOffset");
    for (auto& observer : observers_)
      observer.DidChangeScrollOffset();
  }
}
