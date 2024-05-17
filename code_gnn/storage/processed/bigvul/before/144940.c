void RenderWidgetHostViewAura::OnBeginFrameDidNotSwap(
    const cc::BeginFrameAck& ack) {
  delegated_frame_host_->BeginFrameDidNotSwap(ack);
}
