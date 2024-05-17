void RenderFrameImpl::DidObserveLazyLoadBehavior(
    WebLocalFrameClient::LazyLoadBehavior lazy_load_behavior) {
  for (auto& observer : observers_)
    observer.DidObserveLazyLoadBehavior(lazy_load_behavior);
}
