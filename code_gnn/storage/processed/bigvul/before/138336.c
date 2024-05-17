bool roleAllowsRequired(AccessibilityRole role) {
  return role == ComboBoxRole || role == CellRole || role == ListBoxRole ||
         role == RadioGroupRole || role == SpinButtonRole ||
         role == TextFieldRole || role == TreeRole ||
         role == ColumnHeaderRole || role == RowHeaderRole ||
         role == TreeGridRole;
}
