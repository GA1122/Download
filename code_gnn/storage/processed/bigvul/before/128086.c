void SynchronousCompositorImpl::ReturnResources(
    const cc::CompositorFrameAck& frame_ack) {
  DCHECK(CalledOnValidThread());
  output_surface_->ReturnResources(frame_ack);
}
