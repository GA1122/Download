HTMLLabelElement* AXNodeObject::labelElementContainer() const {
  if (!getNode())
    return 0;

  if (isControl())
    return 0;

  if (isLink())
    return 0;

  return Traversal<HTMLLabelElement>::firstAncestorOrSelf(*getNode());
}
