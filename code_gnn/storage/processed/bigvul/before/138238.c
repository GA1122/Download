AXObject* AXObjectCacheImpl::getOrCreate(Node* node) {
  if (!node)
    return 0;

  if (AXObject* obj = get(node))
    return obj;

  if (node->layoutObject() && !isHTMLAreaElement(node))
    return getOrCreate(node->layoutObject());

  if (!node->parentElement())
    return 0;

  if (isHTMLHeadElement(node))
    return 0;

  AXObject* newObj = createFromNode(node);

  DCHECK(!get(node));

  const AXID axID = getOrCreateAXID(newObj);

  m_nodeObjectMapping.set(node, axID);
  newObj->init();
  newObj->setLastKnownIsIgnoredValue(newObj->accessibilityIsIgnored());

  if (node->isElementNode())
    updateTreeIfElementIdIsAriaOwned(toElement(node));

  return newObj;
}
