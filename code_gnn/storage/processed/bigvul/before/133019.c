void RenderWidgetHostViewAura::SendDelegatedFrameAck(uint32 output_surface_id) {
  cc::CompositorFrameAck ack;
  if (resource_collection_)
    resource_collection_->TakeUnusedResourcesForChildCompositor(&ack.resources);
  RenderWidgetHostImpl::SendSwapCompositorFrameAck(host_->GetRoutingID(),
                                                   output_surface_id,
                                                   host_->GetProcess()->GetID(),
                                                   ack);
  DCHECK_GT(pending_delegated_ack_count_, 0);
  pending_delegated_ack_count_--;
}
