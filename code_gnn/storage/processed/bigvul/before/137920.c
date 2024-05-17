bool AXLayoutObject::canHaveChildren() const {
  if (!m_layoutObject)
    return false;

  return AXNodeObject::canHaveChildren();
}
