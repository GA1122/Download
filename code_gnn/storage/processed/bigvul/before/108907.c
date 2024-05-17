void RenderViewImpl::LoadURLExternally(
    WebKit::WebFrame* frame,
    const WebKit::WebURLRequest& request,
    WebKit::WebNavigationPolicy policy) {
  loadURLExternally(frame, request, policy);
}
