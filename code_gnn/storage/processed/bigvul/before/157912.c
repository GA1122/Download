void RenderViewImpl::DidCommitCompositorFrameForWidget() {
  for (auto& observer : observers_)
    observer.DidCommitCompositorFrame();
  UpdatePreferredSize();
}
