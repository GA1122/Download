void RenderViewImpl::DidCommitCompositorFrame() {
  RenderWidget::DidCommitCompositorFrame();
  for (auto& observer : observers_)
    observer.DidCommitCompositorFrame();
}
