AccessibilityRole AXLayoutObject::determineAccessibilityRole() {
  if (!m_layoutObject)
    return UnknownRole;

  if ((m_ariaRole = determineAriaRoleAttribute()) != UnknownRole)
    return m_ariaRole;

  AccessibilityRole role = nativeAccessibilityRoleIgnoringAria();
  if (role != UnknownRole)
    return role;

  if (m_layoutObject->isLayoutBlockFlow())
    return GroupRole;

  if (supportsARIAAttributes())
    return GroupRole;

  return UnknownRole;
}
