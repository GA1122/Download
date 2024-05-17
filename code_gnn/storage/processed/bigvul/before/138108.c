void AXNodeObject::selectionChanged() {
  if (isFocused() || isWebArea()) {
    axObjectCache().postNotification(this,
                                     AXObjectCacheImpl::AXSelectedTextChanged);
    if (getDocument()) {
      AXObject* documentObject = axObjectCache().getOrCreate(getDocument());
      axObjectCache().postNotification(
          documentObject, AXObjectCacheImpl::AXDocumentSelectionChanged);
    }
  } else {
    AXObject::selectionChanged();   
  }
}
