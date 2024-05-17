bool AXObject::isButton() const {
  AccessibilityRole role = roleValue();

  return role == ButtonRole || role == PopUpButtonRole ||
         role == ToggleButtonRole;
}
