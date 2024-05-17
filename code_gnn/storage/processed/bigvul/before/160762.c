void RenderViewImpl::Close() {
  WebView* doomed = webview_;
  RenderWidget::Close();
  webview_ = nullptr;
  g_view_map.Get().erase(doomed);
  g_routing_id_view_map.Get().erase(GetRoutingID());
  RenderThread::Get()->Send(new ViewHostMsg_Close_ACK(GetRoutingID()));
}
