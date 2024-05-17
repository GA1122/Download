AccessibilityRole AXTableCell::scanToDecideHeaderRole() {
  if (!isTableHeaderCell())
    return CellRole;

  if (isRowHeaderCell())
    return RowHeaderRole;

  if (isColumnHeaderCell())
    return ColumnHeaderRole;

  LayoutTableCell* layoutCell = toLayoutTableCell(m_layoutObject);
  AccessibilityRole headerRole = CellRole;

  if ((headerRole = decideRoleFromSibling(layoutCell->previousCell())) !=
      CellRole)
    return headerRole;

  if ((headerRole = decideRoleFromSibling(layoutCell->nextCell())) != CellRole)
    return headerRole;

  return ColumnHeaderRole;
}
