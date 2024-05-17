bool AXNodeObject::isPasswordField() const {
  Node* node = this->getNode();
  if (!isHTMLInputElement(node))
    return false;

  AccessibilityRole ariaRole = ariaRoleAttribute();
  if (ariaRole != TextFieldRole && ariaRole != UnknownRole)
    return false;

  return toHTMLInputElement(node)->type() == InputTypeNames::password;
}
