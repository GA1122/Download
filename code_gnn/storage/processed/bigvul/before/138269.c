bool AXObjectCacheImpl::nodeIsTextControl(const Node* node) {
  if (!node)
    return false;

  const AXObject* axObject = getOrCreate(const_cast<Node*>(node));
  return axObject && axObject->isTextControl();
}
