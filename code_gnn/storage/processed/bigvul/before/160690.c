void RenderFrameImpl::OnUndo() {
  frame_->ExecuteCommand(WebString::FromUTF8("Undo"));
}
