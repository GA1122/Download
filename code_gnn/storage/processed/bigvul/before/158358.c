void RenderWidgetHostImpl::SendScreenRects() {
  if (!renderer_initialized_ || waiting_for_screen_rects_ack_)
    return;

  if (is_hidden_) {
    return;
  }

  if (!view_)
    return;

  last_view_screen_rect_ = view_->GetViewBounds();
  last_window_screen_rect_ = view_->GetBoundsInRootWindow();
  view_->WillSendScreenRects();
  Send(new ViewMsg_UpdateScreenRects(
      GetRoutingID(), last_view_screen_rect_, last_window_screen_rect_));
  waiting_for_screen_rects_ack_ = true;
}
