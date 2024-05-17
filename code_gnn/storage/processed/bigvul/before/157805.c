bool WebContentsImpl::IsWebContentsOnlyAccessibilityModeForTesting() const {
  return accessibility_mode_ == ui::kAXModeWebContentsOnly;
}
