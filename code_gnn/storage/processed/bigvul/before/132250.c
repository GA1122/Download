void RenderFrameImpl::OnSetAccessibilityMode(AccessibilityMode new_mode) {
  if (accessibility_mode_ == new_mode)
    return;
  accessibility_mode_ = new_mode;
  if (renderer_accessibility_) {
    renderer_accessibility_->DisableAccessibility();

    delete renderer_accessibility_;
    renderer_accessibility_ = NULL;
  }
  if (accessibility_mode_ == AccessibilityModeOff)
    return;

  if (accessibility_mode_ & AccessibilityModeFlagFullTree)
    renderer_accessibility_ = new RendererAccessibility(this);
}
