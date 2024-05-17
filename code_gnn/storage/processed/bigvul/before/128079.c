void SynchronousCompositorImpl::DidStopFlinging() {
  rwhva_->GetRenderWidgetHost()->GetProcess()->OnMessageReceived(
      InputHostMsg_DidStopFlinging(routing_id_));
}
