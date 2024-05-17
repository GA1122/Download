void RenderWidgetHostViewAura::DidCreateNewRendererCompositorFrameSink(
    cc::mojom::MojoCompositorFrameSinkClient* renderer_compositor_frame_sink) {
  renderer_compositor_frame_sink_ = renderer_compositor_frame_sink;
  if (delegated_frame_host_) {
    delegated_frame_host_->DidCreateNewRendererCompositorFrameSink(
        renderer_compositor_frame_sink_);
  }
}
