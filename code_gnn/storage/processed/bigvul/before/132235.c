void RenderFrameImpl::OnDidUpdateSandboxFlags(blink::WebSandboxFlags flags) {
  frame_->setFrameOwnerSandboxFlags(flags);
}
