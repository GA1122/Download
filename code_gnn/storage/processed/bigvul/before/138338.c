bool roleAllowsSort(AccessibilityRole role) {
  return role == ColumnHeaderRole || role == RowHeaderRole;
}
