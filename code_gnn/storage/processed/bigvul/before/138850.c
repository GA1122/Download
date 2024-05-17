void TestRenderWidgetHostView::SubmitCompositorFrame(
    const cc::LocalSurfaceId& local_surface_id,
    cc::CompositorFrame frame) {
  did_swap_compositor_frame_ = true;
}
