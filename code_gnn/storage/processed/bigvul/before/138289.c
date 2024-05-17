void AXObjectCacheImpl::textChanged(AXObject* obj) {
  if (!obj)
    return;

  bool parentAlreadyExists = obj->parentObjectIfExists();
  obj->textChanged();
  postNotification(obj, AXObjectCacheImpl::AXTextChanged);
  if (parentAlreadyExists)
    obj->notifyIfIgnoredValueChanged();
}
