AXObject* AXNodeObject::activeDescendant() {
  if (!m_node || !m_node->isElementNode())
    return nullptr;

  const AtomicString& activeDescendantAttr =
      getAttribute(aria_activedescendantAttr);
  if (activeDescendantAttr.isNull() || activeDescendantAttr.isEmpty())
    return nullptr;

  Element* element = toElement(getNode());
  Element* descendant =
      element->treeScope().getElementById(activeDescendantAttr);
  if (!descendant)
    return nullptr;

  AXObject* axDescendant = axObjectCache().getOrCreate(descendant);
  return axDescendant;
}
