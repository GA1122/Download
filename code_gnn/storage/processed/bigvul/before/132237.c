void RenderFrameImpl::OnExecuteNoValueEditCommand(const std::string& name) {
  frame_->executeCommand(WebString::fromUTF8(name), GetFocusedElement());
}
