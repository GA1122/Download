bool roleAllowsSelected(AccessibilityRole role) {
  return role == CellRole || role == ListBoxOptionRole || role == RowRole ||
         role == TabRole || role == ColumnHeaderRole ||
         role == MenuItemRadioRole || role == RadioButtonRole ||
         role == RowHeaderRole || role == TreeItemRole;
}
