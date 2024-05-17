bool roleAllowsReadonly(AccessibilityRole role) {
  return role == GridRole || role == CellRole || role == TextFieldRole ||
         role == ColumnHeaderRole || role == RowHeaderRole ||
         role == TreeGridRole;
}
