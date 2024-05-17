bool RenderWidgetHostViewAura::CanCopyToBitmap() const {
  return GetCompositor() && window_->layer()->has_external_content();
}
