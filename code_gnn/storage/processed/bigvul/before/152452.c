void RenderFrameImpl::OnSetFocusedFrame() {
  render_view_->webview()->FocusDocumentView(frame_);
}
