bool AXLayoutObject::isValidSelectionBound(const AXObject* boundObject) const {
  return getLayoutObject() && boundObject && !boundObject->isDetached() &&
         boundObject->isAXLayoutObject() && boundObject->getLayoutObject() &&
         boundObject->getLayoutObject()->frame() ==
             getLayoutObject()->frame() &&
         &boundObject->axObjectCache() == &axObjectCache();
}
