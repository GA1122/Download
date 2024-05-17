 cc::SurfaceId RenderWidgetHostViewAura::SurfaceIdForTesting() const {
  return delegated_frame_host_ ? delegated_frame_host_->SurfaceIdForTesting()
                               : cc::SurfaceId();
}
