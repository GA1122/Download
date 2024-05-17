bool roleAllowsMultiselectable(AccessibilityRole role) {
  return role == GridRole || role == ListBoxRole || role == TabListRole ||
         role == TreeGridRole || role == TreeRole;
}
