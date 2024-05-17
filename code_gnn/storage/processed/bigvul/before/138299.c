AXTableCell* AXTableCell::create(LayoutObject* layoutObject,
                                 AXObjectCacheImpl& axObjectCache) {
  return new AXTableCell(layoutObject, axObjectCache);
}
