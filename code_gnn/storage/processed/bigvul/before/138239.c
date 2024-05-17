AXObject* AXObjectCacheImpl::getOrCreate(LayoutObject* layoutObject) {
  if (!layoutObject)
    return 0;

  if (AXObject* obj = get(layoutObject))
    return obj;

  AXObject* newObj = createFromRenderer(layoutObject);

  DCHECK(!get(layoutObject));

  const AXID axid = getOrCreateAXID(newObj);

  m_layoutObjectMapping.set(layoutObject, axid);
  newObj->init();
  newObj->setLastKnownIsIgnoredValue(newObj->accessibilityIsIgnored());

  return newObj;
}
