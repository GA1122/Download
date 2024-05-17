TestRenderWidgetHostView::TestRenderWidgetHostView(RenderWidgetHost* rwh)
    : rwh_(RenderWidgetHostImpl::From(rwh)),
      is_showing_(false),
      is_occluded_(false),
      did_swap_compositor_frame_(false),
      background_color_(SK_ColorWHITE) {
#if defined(OS_ANDROID)
  frame_sink_id_ = AllocateFrameSinkId();
  GetSurfaceManager()->RegisterFrameSinkId(frame_sink_id_);
#else
  if (ImageTransportFactory::GetInstance()) {
    frame_sink_id_ = AllocateFrameSinkId();
    GetSurfaceManager()->RegisterFrameSinkId(frame_sink_id_);
  }
#endif

  rwh_->SetView(this);
}
