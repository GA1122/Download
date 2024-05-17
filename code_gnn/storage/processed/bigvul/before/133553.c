void WebContentsImpl::DidStartLoading(RenderViewHost* render_view_host) {
  SetIsLoading(render_view_host, true, NULL);
}
