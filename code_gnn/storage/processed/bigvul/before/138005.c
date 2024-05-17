AXListBoxOption* AXListBoxOption::create(LayoutObject* layoutObject,
                                         AXObjectCacheImpl& axObjectCache) {
  return new AXListBoxOption(layoutObject, axObjectCache);
}
