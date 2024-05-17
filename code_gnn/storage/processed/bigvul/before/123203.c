bool RenderWidgetHostViewAura::IsSurfaceAvailableForCopy() const {
  return current_surface_ != 0 || !!host_->GetBackingStore(false);
}
