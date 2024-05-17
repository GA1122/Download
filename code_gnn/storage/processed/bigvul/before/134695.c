 void SynchronousCompositorImpl::DidStopFlinging() {
  contents_->GetRenderProcessHost()->OnMessageReceived(
      InputHostMsg_DidStopFlinging(routing_id_));
}
