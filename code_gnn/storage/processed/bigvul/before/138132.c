AccessibilityRole AXObject::buttonRoleType() const {
  if (ariaPressedIsPresent())
    return ToggleButtonRole;
  if (ariaHasPopup())
    return PopUpButtonRole;

  return ButtonRole;
}
