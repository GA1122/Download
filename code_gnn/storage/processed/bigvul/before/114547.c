void WebPluginProxy::OnPaint(const gfx::Rect& damaged_rect) {
  content::GetContentClient()->SetActiveURL(page_url_);

  Paint(damaged_rect);
  bool allow_buffer_flipping;
#if defined(OS_MACOSX)
  allow_buffer_flipping = delegate_->AllowBufferFlipping();
#else
  allow_buffer_flipping = true;
#endif
  Send(new PluginHostMsg_InvalidateRect(route_id_,
                                        damaged_rect,
                                        allow_buffer_flipping));
}
