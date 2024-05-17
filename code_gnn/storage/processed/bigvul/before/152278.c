void RenderFrameImpl::DidHandleOnloadEvents() {
  if (!frame_->Parent()) {
    GetFrameHost()->DocumentOnLoadCompleted();
  }
  for (auto& observer : observers_)
    observer.DidHandleOnloadEvents();
}
