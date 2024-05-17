void AXObjectCacheImpl::handleLoadComplete(Document* document) {
  postNotification(getOrCreate(document), AXObjectCache::AXLoadComplete);
}
