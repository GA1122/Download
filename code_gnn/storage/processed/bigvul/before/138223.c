AXObjectCache* AXObjectCacheImpl::create(Document& document) {
  return new AXObjectCacheImpl(document);
}
