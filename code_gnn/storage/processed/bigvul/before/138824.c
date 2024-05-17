void TestRenderWidgetHostView::DidCreateNewRendererCompositorFrameSink(
    cc::mojom::MojoCompositorFrameSinkClient* renderer_compositor_frame_sink) {
  did_change_compositor_frame_sink_ = true;
}
