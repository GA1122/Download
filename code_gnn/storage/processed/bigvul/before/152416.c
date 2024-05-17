void RenderFrameImpl::OnAdvanceFocus(blink::WebFocusType type,
                                     int32_t source_routing_id) {
  RenderFrameProxy* source_frame =
      RenderFrameProxy::FromRoutingID(source_routing_id);
  if (!source_frame) {
    render_view_->webview()->SetInitialFocus(type ==
                                             blink::kWebFocusTypeBackward);
    return;
  }

  render_view_->webview()->AdvanceFocusAcrossFrames(
      type, source_frame->web_frame(), frame_);
}
