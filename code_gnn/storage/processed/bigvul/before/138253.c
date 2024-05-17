void AXObjectCacheImpl::handleLayoutComplete(Document* document) {
  postNotification(getOrCreate(document), AXObjectCache::AXLayoutComplete);
}
