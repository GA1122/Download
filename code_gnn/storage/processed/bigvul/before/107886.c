void WebViewPlugin::updateGeometry(
    const WebRect& frame_rect, const WebRect& clip_rect,
    const WebVector<WebRect>& cut_out_rects, bool is_visible) {
  if (frame_rect != rect_) {
    rect_ = frame_rect;
    web_view_->resize(WebSize(frame_rect.width, frame_rect.height));
  }
}
