void HTMLFormElement::disassociate(HTMLImageElement& e) {
  m_imageElementsAreDirty = true;
  m_imageElements.clear();
  removeFromPastNamesMap(e);
}
