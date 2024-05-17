void WebContentsImpl::OnUpdateFaviconURL(
    RenderFrameHostImpl* source,
    const std::vector<FaviconURL>& candidates) {
  if (source->GetParent()) {
    NOTREACHED();
    return;
  }

  if (!source->IsCurrent())
    return;

  for (auto& observer : observers_)
    observer.DidUpdateFaviconURL(candidates);
}
