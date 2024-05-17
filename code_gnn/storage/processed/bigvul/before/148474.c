void WebContentsImpl::OnDocumentLoadedInFrame(RenderFrameHostImpl* source) {
  for (auto& observer : observers_)
    observer.DocumentLoadedInFrame(source);
}
