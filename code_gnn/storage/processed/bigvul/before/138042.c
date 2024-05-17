AccessibilityRole AXNodeObject::determineAriaRoleAttribute() const {
  const AtomicString& ariaRole =
      getAOMPropertyOrARIAAttribute(AOMStringProperty::kRole);
  if (ariaRole.isNull() || ariaRole.isEmpty())
    return UnknownRole;

  AccessibilityRole role = ariaRoleToWebCoreRole(ariaRole);

  if ((role == NoneRole || role == PresentationalRole) &&
      canSetFocusAttribute())
    return UnknownRole;

  if (role == ButtonRole)
    role = buttonRoleType();

  role = remapAriaRoleDueToParent(role);

  if (role)
    return role;

  return UnknownRole;
}
