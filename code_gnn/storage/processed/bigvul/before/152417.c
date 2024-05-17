void RenderFrameImpl::OnAdvanceFocusInForm(blink::WebFocusType focus_type) {
  if (render_view_->webview()->FocusedFrame() != frame_)
    return;
  frame_->AdvanceFocusInForm(focus_type);
}
