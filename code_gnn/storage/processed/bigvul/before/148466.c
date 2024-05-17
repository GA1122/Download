void WebContentsImpl::OnDidContainInsecureFormAction(
    RenderFrameHostImpl* source) {
  controller_.ssl_manager()->DidContainInsecureFormAction();
}
