bool AXObjectCacheImpl::accessibilityEnabled() {
  Settings* settings = this->settings();
  if (!settings)
    return false;
  return settings->getAccessibilityEnabled();
}
