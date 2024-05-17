bool RenderWidgetHostViewAura::IsSurfaceAvailableForCopy() const {
  return CanCopyToBitmap() || !!host_->GetBackingStore(false);
}
