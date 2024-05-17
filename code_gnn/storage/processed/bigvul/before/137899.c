void AXARIAGridCell::rowIndexRange(std::pair<unsigned, unsigned>& rowRange) {
  AXObject* parent = parentObjectUnignored();
  if (!parent)
    return;

  if (parent->isTableRow()) {
    rowRange.first = toAXTableRow(parent)->rowIndex();
  } else if (parent->isAXTable()) {
    unsigned columnCount = toAXTable(parent)->columnCount();
    if (!columnCount)
      return;

    const auto& siblings = parent->children();
    unsigned childrenSize = siblings.size();
    for (unsigned k = 0; k < childrenSize; ++k) {
      if (siblings[k].get() == this) {
        rowRange.first = k / columnCount;
        break;
      }
    }
  }

  rowRange.second = 1;
}
