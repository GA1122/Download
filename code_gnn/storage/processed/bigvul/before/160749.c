void RenderFrameImpl::WillCommitProvisionalLoad() {
  SCOPED_UMA_HISTOGRAM_TIMER("RenderFrameObservers.WillCommitProvisionalLoad");
  for (auto& observer : observers_)
    observer.WillCommitProvisionalLoad();
}
