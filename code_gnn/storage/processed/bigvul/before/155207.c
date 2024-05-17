void HTMLFormElement::associate(HTMLImageElement& e) {
  m_imageElementsAreDirty = true;
  m_imageElements.clear();
}
