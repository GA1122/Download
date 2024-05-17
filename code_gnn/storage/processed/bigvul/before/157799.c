bool WebContentsImpl::IsFullAccessibilityModeForTesting() const {
  return accessibility_mode_ == ui::kAXModeComplete;
}
