AXObject* AXObjectCacheImpl::root() {
  return getOrCreate(m_document);
}
