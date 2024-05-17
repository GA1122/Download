AXARIAGridCell* AXARIAGridCell::create(LayoutObject* layoutObject,
                                       AXObjectCacheImpl& axObjectCache) {
  return new AXARIAGridCell(layoutObject, axObjectCache);
}
