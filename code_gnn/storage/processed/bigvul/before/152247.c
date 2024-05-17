void RenderFrameImpl::DidCancelResponse(int request_id) {
  for (auto& observer : observers_)
    observer.DidCancelResponse(request_id);
}
