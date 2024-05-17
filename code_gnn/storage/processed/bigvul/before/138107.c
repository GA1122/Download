AccessibilityRole AXNodeObject::remapAriaRoleDueToParent(
    AccessibilityRole role) const {

  if (role != ListBoxOptionRole && role != MenuItemRole)
    return role;

  for (AXObject* parent = parentObject();
       parent && !parent->accessibilityIsIgnored();
       parent = parent->parentObject()) {
    AccessibilityRole parentAriaRole = parent->ariaRoleAttribute();

    if (role == ListBoxOptionRole && parentAriaRole == MenuRole)
      return MenuItemRole;
    if (role == MenuItemRole && parentAriaRole == GroupRole)
      return MenuButtonRole;

    if (parentAriaRole)
      break;
  }

  return role;
}
