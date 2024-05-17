void WebContentsImpl::RenderViewDeleted(RenderViewHost* rvh) {
  for (auto& observer : observers_)
    observer.RenderViewDeleted(rvh);
}
