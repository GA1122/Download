void WebPluginProxy::OnPaint(const gfx::Rect& damaged_rect) {
  child_process_logging::SetActiveURL(page_url_);

  Paint(damaged_rect);
  Send(new PluginHostMsg_InvalidateRect(route_id_, damaged_rect));
}
