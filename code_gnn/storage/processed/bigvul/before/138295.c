unsigned AXTableCell::ariaColumnIndex() const {
  const AtomicString& colIndex = getAttribute(aria_colindexAttr);
  if (colIndex.toInt() >= 1)
    return colIndex.toInt();

  AXObject* parent = parentObjectUnignored();
  if (!parent || !parent->isTableRow())
    return 0;

  return m_ariaColIndexFromRow;
}
