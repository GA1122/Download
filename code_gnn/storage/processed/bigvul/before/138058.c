bool AXNodeObject::isControl() const {
  Node* node = this->getNode();
  if (!node)
    return false;

  return ((node->isElementNode() && toElement(node)->isFormControlElement()) ||
          AXObject::isARIAControl(ariaRoleAttribute()));
}
