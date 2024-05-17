Element* AXNodeObject::anchorElement() const {
  Node* node = this->getNode();
  if (!node)
    return 0;

  AXObjectCacheImpl& cache = axObjectCache();

  for (; node; node = node->parentNode()) {
    if (isHTMLAnchorElement(*node) ||
        (node->layoutObject() &&
         cache.getOrCreate(node->layoutObject())->isAnchor()))
      return toElement(node);
  }

  return 0;
}
