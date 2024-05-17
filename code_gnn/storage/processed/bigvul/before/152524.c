void RenderFrameImpl::SetLifecycleState(
    blink::mojom::FrameLifecycleState state) {
  frame_->SetLifecycleState(state);
}
