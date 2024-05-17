AXObject* AXTableCell::parentTable() const {
  if (!m_layoutObject || !m_layoutObject->isTableCell())
    return 0;

  if (isDetached())
    return 0;

  return axObjectCache().get(toLayoutTableCell(m_layoutObject)->table());
}
