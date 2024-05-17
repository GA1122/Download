void RenderFrameImpl::OnDidUpdateFramePolicy(
    const blink::FramePolicy& frame_policy) {
  frame_->SetFrameOwnerPolicy(frame_policy.sandbox_flags,
                              frame_policy.container_policy);
}
