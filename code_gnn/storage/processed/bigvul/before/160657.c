void RenderFrameImpl::OnExecuteNoValueEditCommand(const std::string& name) {
  frame_->ExecuteCommand(WebString::FromUTF8(name));
}
