bool AXNodeObject::isEnabled() const {
  if (isDescendantOfDisabledNode())
    return false;

  Node* node = this->getNode();
  if (!node || !node->isElementNode())
    return true;

  return !toElement(node)->isDisabledFormControl();
}
