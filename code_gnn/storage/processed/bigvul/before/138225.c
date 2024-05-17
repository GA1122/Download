AXObject* AXObjectCacheImpl::createFromNode(Node* node) {
  if (isMenuListOption(node))
    return AXMenuListOption::create(toHTMLOptionElement(node), *this);

  if (isHTMLAreaElement(node))
    return AXImageMapLink::create(toHTMLAreaElement(node), *this);

  return AXNodeObject::create(node, *this);
}
