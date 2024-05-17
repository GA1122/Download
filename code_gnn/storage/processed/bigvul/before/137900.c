AccessibilityRole AXARIAGridCell::scanToDecideHeaderRole() {
  if (isAriaRowHeader())
    return RowHeaderRole;

  if (isAriaColumnHeader())
    return ColumnHeaderRole;

  return CellRole;
}
