SurfaceHitTestReadyNotifier::SurfaceHitTestReadyNotifier(
    RenderWidgetHostViewBase* target_view)
    : target_view_(target_view) {
  surface_manager_ = GetFrameSinkManager()->surface_manager();
}
