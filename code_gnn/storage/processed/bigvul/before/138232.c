AXObject* AXObjectCacheImpl::focusedObject() {
  if (!accessibilityEnabled())
    return nullptr;

  Node* focusedNode = m_document->focusedElement();
  if (!focusedNode)
    focusedNode = m_document;

  if (isHTMLAreaElement(focusedNode))
    return focusedImageMapUIElement(toHTMLAreaElement(focusedNode));

  Element* adjustedFocusedElement = m_document->adjustedFocusedElement();
  if (isHTMLInputElement(adjustedFocusedElement)) {
    if (AXObject* axPopup =
            toHTMLInputElement(adjustedFocusedElement)->popupRootAXObject()) {
      if (Element* focusedElementInPopup =
              axPopup->getDocument()->focusedElement())
        focusedNode = focusedElementInPopup;
    }
  }

  AXObject* obj = getOrCreate(focusedNode);
  if (!obj)
    return nullptr;

  if (obj->accessibilityIsIgnored())
    obj = obj->parentObjectUnignored();

  return obj;
}
