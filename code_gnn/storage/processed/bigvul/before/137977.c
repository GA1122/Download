AXObject* AXLayoutObject::rawFirstChild() const {
  if (!m_layoutObject)
    return 0;

  LayoutObject* firstChild = firstChildConsideringContinuation(m_layoutObject);

  if (!firstChild)
    return 0;

  return axObjectCache().getOrCreate(firstChild);
}
