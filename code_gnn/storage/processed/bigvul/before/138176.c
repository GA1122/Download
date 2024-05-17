bool AXObject::isPasswordFieldAndShouldHideValue() const {
  Settings* settings = getDocument()->settings();
  if (!settings || settings->getAccessibilityPasswordValuesEnabled())
    return false;

  return isPasswordField();
}
