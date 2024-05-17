bool AutofillExternalDelegate::HasActiveScreenReader() const {
  return ui::AXPlatformNode::GetAccessibilityMode().has_mode(
      ui::AXMode::kScreenReader);
}
