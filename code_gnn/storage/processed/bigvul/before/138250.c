void AXObjectCacheImpl::handleFocusedUIElementChanged(Node* oldFocusedNode,
                                                      Node* newFocusedNode) {
  if (!newFocusedNode)
    return;

  Page* page = newFocusedNode->document().page();
  if (!page)
    return;

  AXObject* focusedObject = this->focusedObject();
  if (!focusedObject)
    return;

  AXObject* oldFocusedObject = get(oldFocusedNode);

  postPlatformNotification(oldFocusedObject, AXBlur);
  postPlatformNotification(focusedObject, AXFocusedUIElementChanged);
}
