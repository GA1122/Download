bool AXNodeObject::isReadOnly() const {
  Node* node = this->getNode();
  if (!node)
    return true;

  if (isHTMLTextAreaElement(*node))
    return toHTMLTextAreaElement(*node).isReadOnly();

  if (isHTMLInputElement(*node)) {
    HTMLInputElement& input = toHTMLInputElement(*node);
    if (input.isTextField())
      return input.isReadOnly();
  }

  return !hasEditableStyle(*node);
}
