RenderFrameImpl::CreateApplicationCacheHost(
    blink::WebDocumentLoader* document_loader,
    blink::WebApplicationCacheHostClient* client) {
  if (!frame_ || !frame_->View())
    return nullptr;

  NavigationState* navigation_state =
      NavigationState::FromDocumentLoader(document_loader);

  scoped_refptr<base::SingleThreadTaskRunner> task_runner =
      frame_->GetTaskRunner(blink::TaskType::kNetworking);

  return std::make_unique<RendererWebApplicationCacheHostImpl>(
      RenderViewImpl::FromWebView(frame_->View()), client,
      navigation_state->commit_params().appcache_host_id, routing_id_,
      std::move(task_runner));
}
