void WebPluginImpl::paint(WebCanvas* canvas, const WebRect& paint_rect) {
  if (!delegate_)
    return;

  delegate_->Paint(canvas, paint_rect);
}
