AXObject* AXObject::parentObject() const {
  if (isDetached())
    return 0;

  if (m_parent)
    return m_parent;

  if (axObjectCache().isAriaOwned(this))
    return axObjectCache().getAriaOwnedParent(this);

  return computeParent();
}
