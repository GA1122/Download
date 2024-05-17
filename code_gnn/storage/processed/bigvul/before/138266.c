void AXObjectCacheImpl::listboxActiveIndexChanged(HTMLSelectElement* select) {
  AXObject* obj = get(select);
  if (!obj || !obj->isAXListBox())
    return;

  toAXListBox(obj)->activeIndexChanged();
}
