AXLayoutObject* AXLayoutObject::getUnignoredObjectFromNode(Node& node) const {
  if (isDetached())
    return nullptr;

  AXObject* axObject = axObjectCache().getOrCreate(&node);
  if (!axObject)
    return nullptr;

  if (axObject->isAXLayoutObject() && !axObject->accessibilityIsIgnored())
    return toAXLayoutObject(axObject);

  return nullptr;
}
