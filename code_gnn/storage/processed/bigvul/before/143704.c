void RenderWidgetHostImpl::SendReclaimCompositorResources(
    int32_t route_id,
    uint32_t output_surface_id,
    int renderer_host_id,
    const cc::CompositorFrameAck& ack) {
  RenderProcessHost* host = RenderProcessHost::FromID(renderer_host_id);
  if (!host)
    return;
  host->Send(
      new ViewMsg_ReclaimCompositorResources(route_id, output_surface_id, ack));
}
