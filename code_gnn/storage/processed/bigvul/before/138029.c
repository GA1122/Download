bool AXNodeObject::canSetSelectedAttribute() const {
  if (ariaRoleAttribute() == ListBoxOptionRole &&
      ancestorExposesActiveDescendant())
    return true;
  return AXObject::canSetSelectedAttribute();
}
