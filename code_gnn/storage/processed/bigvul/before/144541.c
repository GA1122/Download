RenderViewHostImpl* WebContentsImpl::GetRenderViewHost() const {
  return GetRenderManager()->current_host();
}
