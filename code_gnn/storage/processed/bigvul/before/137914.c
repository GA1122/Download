Element* AXLayoutObject::anchorElement() const {
  if (!m_layoutObject)
    return 0;

  AXObjectCacheImpl& cache = axObjectCache();
  LayoutObject* currLayoutObject;

  for (currLayoutObject = m_layoutObject;
       currLayoutObject && !currLayoutObject->node();
       currLayoutObject = currLayoutObject->parent()) {
    if (currLayoutObject->isAnonymousBlock() &&
        currLayoutObject->isLayoutBlockFlow()) {
      LayoutObject* continuation =
          toLayoutBlockFlow(currLayoutObject)->continuation();
      if (continuation)
        return cache.getOrCreate(continuation)->anchorElement();
    }
  }

  if (!currLayoutObject)
    return 0;

  Node* node = currLayoutObject->node();
  if (!node)
    return nullptr;
  for (Node& runner : NodeTraversal::inclusiveAncestorsOf(*node)) {
    if (isHTMLAnchorElement(runner) ||
        (runner.layoutObject() &&
         cache.getOrCreate(runner.layoutObject())->isAnchor()))
      return toElement(&runner);
  }

  return 0;
}
