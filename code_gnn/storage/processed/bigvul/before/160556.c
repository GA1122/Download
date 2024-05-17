RenderFrameImpl::CreateApplicationCacheHost(
    blink::WebApplicationCacheHostClient* client) {
  if (!frame_ || !frame_->View())
    return nullptr;

  DocumentState* document_state =
      frame_->GetProvisionalDocumentLoader()
          ? DocumentState::FromDocumentLoader(
                frame_->GetProvisionalDocumentLoader())
          : DocumentState::FromDocumentLoader(frame_->GetDocumentLoader());

  NavigationStateImpl* navigation_state =
      static_cast<NavigationStateImpl*>(document_state->navigation_state());

  return std::make_unique<RendererWebApplicationCacheHostImpl>(
      RenderViewImpl::FromWebView(frame_->View()), client,
      RenderThreadImpl::current()->appcache_dispatcher()->backend_proxy(),
      navigation_state->request_params().appcache_host_id, routing_id_);
}
