bool AXNodeObject::isNativeCheckboxOrRadio() const {
  Node* node = this->getNode();
  if (!isHTMLInputElement(node))
    return false;

  HTMLInputElement* input = toHTMLInputElement(node);
  return input->type() == InputTypeNames::checkbox ||
         input->type() == InputTypeNames::radio;
}
