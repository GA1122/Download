void AXARIAGridCell::columnIndexRange(
    std::pair<unsigned, unsigned>& columnRange) {
  AXObject* parent = parentObjectUnignored();
  if (!parent)
    return;

  if (!parent->isTableRow() && !parent->isAXTable())
    return;

  const auto& siblings = parent->children();
  unsigned childrenSize = siblings.size();
  for (unsigned k = 0; k < childrenSize; ++k) {
    if (siblings[k].get() == this) {
      columnRange.first = k;
      break;
    }
  }

  columnRange.second = 1;
}
