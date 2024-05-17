void LayerTreeHost::DidFailToInitializeCompositorFrameSink() {
  DCHECK(new_compositor_frame_sink_);
  current_compositor_frame_sink_ = nullptr;
  new_compositor_frame_sink_ = nullptr;
  client_->DidFailToInitializeCompositorFrameSink();
}
