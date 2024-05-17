static AccessibilityRole decideRoleFromSibling(LayoutTableCell* siblingCell) {
  if (!siblingCell)
    return CellRole;

  if (Node* siblingNode = siblingCell->node()) {
    if (siblingNode->hasTagName(thTag))
      return ColumnHeaderRole;
    if (siblingNode->hasTagName(tdTag))
      return RowHeaderRole;
  }

  return CellRole;
}
