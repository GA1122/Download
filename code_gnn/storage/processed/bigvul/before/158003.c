void RenderViewImpl::UpdatePreferredSize() {
  if (!send_preferred_size_changes_ || !webview())
    return;

  if (!needs_preferred_size_update_)
    return;
  needs_preferred_size_update_ = false;

  blink::WebSize tmp_size = webview()->ContentsPreferredMinimumSize();
  blink::WebRect tmp_rect(0, 0, tmp_size.width, tmp_size.height);
  WidgetClient()->ConvertViewportToWindow(&tmp_rect);
  gfx::Size size(tmp_rect.width, tmp_rect.height);
  if (size == preferred_size_)
    return;

  preferred_size_ = size;
  Send(new ViewHostMsg_DidContentsPreferredSizeChange(GetRoutingID(),
                                                      preferred_size_));
}
