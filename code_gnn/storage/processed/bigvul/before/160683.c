void RenderFrameImpl::OnSetAccessibilityMode(ui::AXMode new_mode) {
  if (accessibility_mode_ == new_mode)
    return;
  ui::AXMode old_mode = accessibility_mode_;
  accessibility_mode_ = new_mode;

  if (new_mode.has_mode(ui::AXMode::kWebContents) &&
      !old_mode.has_mode(ui::AXMode::kWebContents)) {
    render_accessibility_ = new RenderAccessibilityImpl(this, new_mode);
  } else if (!new_mode.has_mode(ui::AXMode::kWebContents) &&
             old_mode.has_mode(ui::AXMode::kWebContents)) {
    render_accessibility_->DisableAccessibility();
    delete render_accessibility_;
    render_accessibility_ = nullptr;
  }

  for (auto& observer : observers_)
    observer.AccessibilityModeChanged();
}
