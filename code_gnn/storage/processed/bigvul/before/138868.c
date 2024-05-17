TestRenderWidgetHostView::~TestRenderWidgetHostView() {
  cc::SurfaceManager* manager = GetSurfaceManager();
  if (manager) {
    manager->InvalidateFrameSinkId(frame_sink_id_);
  }
}
