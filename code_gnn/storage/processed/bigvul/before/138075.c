bool AXNodeObject::isNativeCheckboxInMixedState() const {
  if (!isHTMLInputElement(m_node))
    return false;

  HTMLInputElement* input = toHTMLInputElement(m_node);
  return input->type() == InputTypeNames::checkbox &&
         input->shouldAppearIndeterminate();
}
