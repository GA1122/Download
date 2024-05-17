void CompositorImpl::TearDownDisplayAndUnregisterRootFrameSink() {
  SCOPED_UMA_HISTOGRAM_LONG_TIMER("CompositorImplAndroid.TearDownDisplayTime");

  if (enable_viz_) {
    if (display_private_ && HavePendingReadbacks()) {
      display_private_->ForceImmediateDrawAndSwapIfPossible();
    }

    GetHostFrameSinkManager()->InvalidateFrameSinkId(frame_sink_id_);
    display_private_.reset();
  } else {
    if (display_ && HavePendingReadbacks())
      display_->ForceImmediateDrawAndSwapIfPossible();

    if (display_) {
      GetFrameSinkManager()->UnregisterBeginFrameSource(
          root_window_->GetBeginFrameSource());
    }

    GetHostFrameSinkManager()->InvalidateFrameSinkId(frame_sink_id_);
    display_.reset();
  }
}
