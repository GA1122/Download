const HeapVector<Member<HTMLImageElement>>& HTMLFormElement::imageElements() {
  if (!m_imageElementsAreDirty)
    return m_imageElements;
  collectImageElements(m_hasElementsAssociatedByParser
                           ? NodeTraversal::highestAncestorOrSelf(*this)
                           : *this,
                       m_imageElements);
  m_imageElementsAreDirty = false;
  return m_imageElements;
}
