void LayerTreeHostImpl::ReleaseCompositorFrameSink() {
  TRACE_EVENT0("cc", "LayerTreeHostImpl::ReleaseCompositorFrameSink");

  if (!compositor_frame_sink_) {
    DCHECK(!has_valid_compositor_frame_sink_);
    return;
  }

  has_valid_compositor_frame_sink_ = false;

  ReleaseTreeResources();

  CleanUpTileManagerAndUIResources();
  resource_provider_ = nullptr;

  SetContextVisibility(false);

  compositor_frame_sink_->DetachFromClient();
  compositor_frame_sink_ = nullptr;

  use_gpu_rasterization_ = false;
}
