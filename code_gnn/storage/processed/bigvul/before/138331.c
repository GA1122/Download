bool roleAllowsChecked(AccessibilityRole role) {
  return role == MenuItemCheckBoxRole || role == MenuItemRadioRole ||
         role == RadioButtonRole || role == CheckBoxRole ||
         role == TreeItemRole || role == ListBoxOptionRole ||
         role == SwitchRole;
}
