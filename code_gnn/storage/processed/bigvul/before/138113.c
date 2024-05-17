String AXNodeObject::text() const {
  if (ariaRoleAttribute() == StaticTextRole)
    return computedName();

  if (!isTextControl())
    return String();

  Node* node = this->getNode();
  if (!node)
    return String();

  if (isNativeTextControl() &&
      (isHTMLTextAreaElement(*node) || isHTMLInputElement(*node)))
    return toTextControlElement(*node).value();

  if (!node->isElementNode())
    return String();

  return toElement(node)->innerText();
}
