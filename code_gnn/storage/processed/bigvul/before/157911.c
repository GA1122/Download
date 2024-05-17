void RenderViewImpl::DidCloseWidget() {
  g_view_map.Get().erase(webview_);
  webview_ = nullptr;
  g_routing_id_view_map.Get().erase(GetRoutingID());
}
