bool RenderWidgetHostViewAura::IsSurfaceAvailableForCopy() const {
  if (!delegated_frame_host_)
    return false;
  return delegated_frame_host_->CanCopyFromCompositingSurface();
}
