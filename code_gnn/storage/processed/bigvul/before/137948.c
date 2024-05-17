int AXLayoutObject::index(const VisiblePosition& position) const {
  if (position.isNull() || !isTextControl())
    return -1;

  if (layoutObjectContainsPosition(m_layoutObject, position.deepEquivalent()))
    return indexForVisiblePosition(position);

  return -1;
}
