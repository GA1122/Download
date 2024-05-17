void RenderWidgetHostViewAura::SendReturnedDelegatedResources(
    uint32 output_surface_id) {
  cc::CompositorFrameAck ack;
  if (resource_collection_)
    resource_collection_->TakeUnusedResourcesForChildCompositor(&ack.resources);
  DCHECK(!ack.resources.empty());
  RenderWidgetHostImpl::SendReclaimCompositorResources(
      host_->GetRoutingID(),
      output_surface_id,
      host_->GetProcess()->GetID(),
      ack);
}
