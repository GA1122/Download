void AXTableCell::rowIndexRange(std::pair<unsigned, unsigned>& rowRange) {
  if (!m_layoutObject || !m_layoutObject->isTableCell())
    return;

  LayoutTableCell* layoutCell = toLayoutTableCell(m_layoutObject);
  rowRange.first = layoutCell->rowIndex();
  rowRange.second = layoutCell->rowSpan();

  LayoutTableSection* section = layoutCell->section();
  LayoutTable* table = layoutCell->table();
  if (!table || !section)
    return;

  LayoutTableSection* tableSection = table->topSection();
  unsigned rowOffset = 0;
  while (tableSection) {
    if (tableSection == section)
      break;
    rowOffset += tableSection->numRows();
    tableSection = table->sectionBelow(tableSection, SkipEmptySections);
  }

  rowRange.first += rowOffset;
}
