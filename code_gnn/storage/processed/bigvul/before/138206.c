bool AXObject::supportsSetSizeAndPosInSet() const {
  AXObject* parent = parentObject();
  if (!parent)
    return false;

  int role = roleValue();
  int parentRole = parent->roleValue();

  if ((role == ListBoxOptionRole && parentRole == ListBoxRole) ||
      (role == ListItemRole && parentRole == ListRole) ||
      (role == MenuItemRole && parentRole == MenuRole) ||
      (role == RadioButtonRole) ||
      (role == TabRole && parentRole == TabListRole) ||
      (role == TreeItemRole && parentRole == TreeRole) ||
      (role == TreeItemRole && parentRole == GroupRole)) {
    return true;
  }

  return false;
}
