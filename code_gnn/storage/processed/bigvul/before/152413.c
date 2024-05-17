void RenderFrameImpl::NotifyObserversOfFailedProvisionalLoad(
    const blink::WebURLError& error) {
  for (auto& observer : render_view_->observers())
    observer.DidFailProvisionalLoad(frame_, error);
  for (auto& observer : observers_)
    observer.DidFailProvisionalLoad(error);
}
