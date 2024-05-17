void RenderViewImpl::OnSetAccessibilityMode(AccessibilityMode new_mode) {
  if (accessibility_mode_ == new_mode)
    return;
  accessibility_mode_ = new_mode;
  if (renderer_accessibility_) {
    delete renderer_accessibility_;
    renderer_accessibility_ = NULL;
  }
  if (accessibility_mode_ == AccessibilityModeComplete)
    renderer_accessibility_ = new RendererAccessibilityComplete(this);
  else if (accessibility_mode_ == AccessibilityModeEditableTextOnly)
    renderer_accessibility_ = new RendererAccessibilityFocusOnly(this);
}
