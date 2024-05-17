void RenderFrameImpl::OnRedo() {
  frame_->ExecuteCommand(WebString::FromUTF8("Redo"));
}
