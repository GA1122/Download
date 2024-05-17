bool AXTableCell::isTableCell() const {
  AXObject* parent = parentObjectUnignored();
  if (!parent || !parent->isTableRow())
    return false;

  return true;
}
