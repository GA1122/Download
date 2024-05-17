String AXNodeObject::placeholderFromNativeAttribute() const {
  Node* node = getNode();
  if (!node || !isTextControlElement(node))
    return String();
  return toTextControlElement(node)->strippedPlaceholder();
}
