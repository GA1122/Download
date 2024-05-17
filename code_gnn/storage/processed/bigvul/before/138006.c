AccessibilityRole AXListBoxOption::determineAccessibilityRole() {
  if ((m_ariaRole = determineAriaRoleAttribute()) != UnknownRole)
    return m_ariaRole;

  if (isParentPresentationalRole())
    return StaticTextRole;

  return ListBoxOptionRole;
}
