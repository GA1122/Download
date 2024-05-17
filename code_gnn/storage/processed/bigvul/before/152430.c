void RenderFrameImpl::OnDidUpdateFramePolicy(
    const blink::FramePolicy& frame_policy) {
  frame_->SetFrameOwnerPolicy(frame_policy);
}
