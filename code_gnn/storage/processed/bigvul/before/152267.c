void RenderFrameImpl::DidCreateNewDocument() {
  for (auto& observer : observers_)
    observer.DidCreateNewDocument();
}
