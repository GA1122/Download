void RenderFrameImpl::WillCommitProvisionalLoad() {
  for (auto& observer : observers_)
    observer.WillCommitProvisionalLoad();
}
