WebRect RenderWidget::rootWindowRect() {
  if (pending_window_rect_count_) {
    return pending_window_rect_;
  }

  gfx::Rect rect;
  Send(new ViewHostMsg_GetRootWindowRect(routing_id_, host_window_, &rect));
  return rect;
}
