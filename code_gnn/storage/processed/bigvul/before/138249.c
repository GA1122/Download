void AXObjectCacheImpl::handleEditableTextContentChanged(Node* node) {
  AXObject* obj = get(node);
  while (obj && !obj->isNativeTextControl() && !obj->isNonNativeTextControl())
    obj = obj->parentObject();
  postNotification(obj, AXObjectCache::AXValueChanged);
}
