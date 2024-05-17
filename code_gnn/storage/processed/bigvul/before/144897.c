void RenderWidgetHostViewAura::EndFrameSubscription() {
  if (delegated_frame_host_)
    delegated_frame_host_->EndFrameSubscription();
}
