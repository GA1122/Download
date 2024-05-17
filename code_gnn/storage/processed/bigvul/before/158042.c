void LocalFrameClientImpl::DidChangeFramePolicy(
    Frame* child_frame,
    SandboxFlags flags,
    const ParsedFeaturePolicy& container_policy) {
  if (!web_frame_->Client())
    return;
  web_frame_->Client()->DidChangeFramePolicy(
      WebFrame::FromFrame(child_frame), static_cast<WebSandboxFlags>(flags),
      container_policy);
}
