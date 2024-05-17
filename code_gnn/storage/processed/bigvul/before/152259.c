void RenderFrameImpl::DidChangeThemeColor() {
  if (frame_->Parent())
    return;

  Send(new FrameHostMsg_DidChangeThemeColor(
      routing_id_, frame_->GetDocument().ThemeColor()));
}
