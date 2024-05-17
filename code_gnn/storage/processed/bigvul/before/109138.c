void RenderViewImpl::loadURLExternally(
    WebFrame* frame, const WebURLRequest& request,
    WebNavigationPolicy policy) {
  loadURLExternally(frame, request, policy, WebString());
}
