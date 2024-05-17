void RenderWidgetHostViewAura::BeginFrameSubscription(
    std::unique_ptr<RenderWidgetHostViewFrameSubscriber> subscriber) {
  if (delegated_frame_host_)
    delegated_frame_host_->BeginFrameSubscription(std::move(subscriber));
}
