void LayerTreeHost::DidInitializeCompositorFrameSink() {
  DCHECK(new_compositor_frame_sink_);
  current_compositor_frame_sink_ = std::move(new_compositor_frame_sink_);
  client_->DidInitializeCompositorFrameSink();
}
