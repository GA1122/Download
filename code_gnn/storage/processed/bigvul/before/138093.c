Element* AXNodeObject::menuItemElementForMenu() const {
  if (ariaRoleAttribute() != MenuRole)
    return 0;

  return siblingWithAriaRole("menuitem", getNode());
}
