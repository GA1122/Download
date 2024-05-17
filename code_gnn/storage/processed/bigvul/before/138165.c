bool AXObject::isARIAControl(AccessibilityRole ariaRole) {
  return isARIAInput(ariaRole) || ariaRole == ButtonRole ||
         ariaRole == ComboBoxRole || ariaRole == SliderRole;
}
