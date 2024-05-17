void WebPluginDelegateProxy::OnNotifyIMEStatus(int input_type,
                                               const gfx::Rect& caret_rect) {
  if (!render_view_)
    return;

  render_view_->Send(new ViewHostMsg_TextInputStateChanged(
      render_view_->routing_id(),
      static_cast<ui::TextInputType>(input_type),
      true));

  render_view_->Send(new ViewHostMsg_SelectionBoundsChanged(
      render_view_->routing_id(), caret_rect, caret_rect));
}
