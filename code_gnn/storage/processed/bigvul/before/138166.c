bool AXObject::isARIAInput(AccessibilityRole ariaRole) {
  return ariaRole == RadioButtonRole || ariaRole == CheckBoxRole ||
         ariaRole == TextFieldRole || ariaRole == SwitchRole ||
         ariaRole == SearchBoxRole;
}
