WebContentsImpl::PauseSubresourceLoading() {
  std::vector<blink::mojom::PauseSubresourceLoadingHandlePtr> handles;
  for (RenderFrameHost* rfh : GetAllFrames()) {
    if (!rfh->IsRenderFrameLive())
      continue;
    handles.push_back(rfh->PauseSubresourceLoading());
  }
  return handles;
}
