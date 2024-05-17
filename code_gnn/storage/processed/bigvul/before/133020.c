void RenderWidgetHostViewAura::SendReclaimSoftwareFrames() {
  if (!released_software_frame_)
    return;
  cc::CompositorFrameAck ack;
  ack.last_software_frame_id = released_software_frame_->frame_id;
  RenderWidgetHostImpl::SendReclaimCompositorResources(
      host_->GetRoutingID(),
      released_software_frame_->output_surface_id,
      host_->GetProcess()->GetID(),
      ack);
  released_software_frame_.reset();
}
