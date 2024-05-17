bool AXLayoutObject::isLoaded() const {
  return !m_layoutObject->document().parser();
}
