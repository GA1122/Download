AXLayoutObject* AXLayoutObject::create(LayoutObject* layoutObject,
                                       AXObjectCacheImpl& axObjectCache) {
  return new AXLayoutObject(layoutObject, axObjectCache);
}
