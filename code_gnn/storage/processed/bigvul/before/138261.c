bool AXObjectCacheImpl::inlineTextBoxAccessibilityEnabled() {
  Settings* settings = this->settings();
  if (!settings)
    return false;
  return settings->getInlineTextBoxAccessibilityEnabled();
}
