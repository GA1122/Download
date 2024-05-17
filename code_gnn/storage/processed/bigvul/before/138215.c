DEFINE_TRACE(AXObjectCacheImpl) {
  visitor->trace(m_document);
  visitor->trace(m_nodeObjectMapping);

  visitor->trace(m_objects);
  visitor->trace(m_notificationsToPost);

  AXObjectCache::trace(visitor);
}
