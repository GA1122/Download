void RenderFrameImpl::didChangeThemeColor() {
  if (frame_->parent())
    return;

  Send(new FrameHostMsg_DidChangeThemeColor(
      routing_id_, frame_->document().themeColor()));
}
