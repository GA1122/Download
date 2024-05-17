void AXObjectCacheImpl::handleInitialFocus() {
  postNotification(m_document, AXObjectCache::AXFocusedUIElementChanged);
}
