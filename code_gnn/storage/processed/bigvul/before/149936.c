void LayerTreeHostImpl::DidReceiveCompositorFrameAck() {
  client_->DidReceiveCompositorFrameAckOnImplThread();
}
