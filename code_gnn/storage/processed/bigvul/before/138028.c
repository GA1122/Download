bool AXNodeObject::canSetFocusAttribute() const {
  Node* node = getNode();
  if (!node)
    return false;

  if (isWebArea())
    return true;

  if (!isPresentational() && ariaRoleAttribute() != UnknownRole &&
      ancestorExposesActiveDescendant())
    return true;

  if (isDisabledFormControl(node))
    return false;

  return node->isElementNode() && toElement(node)->supportsFocus();
 }
