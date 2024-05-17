void SynchronousCompositorOutputSurface::ReturnResources(
    const cc::CompositorFrameAck& frame_ack) {
  ReclaimResources(&frame_ack);
}
