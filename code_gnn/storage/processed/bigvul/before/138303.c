bool AXTableCell::isTableHeaderCell() const {
  return getNode() && getNode()->hasTagName(thTag);
}
