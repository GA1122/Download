void WebContentsImpl::DidReceiveCompositorFrame() {
  for (auto& observer : observers_)
    observer.DidReceiveCompositorFrame();
}
