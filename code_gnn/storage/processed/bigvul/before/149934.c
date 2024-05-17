void LayerTreeHostImpl::DidLoseCompositorFrameSink() {
  if (resource_provider_)
    resource_provider_->DidLoseContextProvider();
  has_valid_compositor_frame_sink_ = false;
  client_->DidLoseCompositorFrameSinkOnImplThread();
}
