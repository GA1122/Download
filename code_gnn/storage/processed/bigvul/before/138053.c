const AXObject* AXNodeObject::inheritsPresentationalRoleFrom() const {
  if (canSetFocusAttribute())
    return 0;

  if (isPresentational())
    return this;

  if (ariaRoleAttribute() != UnknownRole)
    return 0;

  AXObject* parent = parentObject();
  if (!parent)
    return 0;

  HTMLElement* element = nullptr;
  if (getNode() && getNode()->isHTMLElement())
    element = toHTMLElement(getNode());
  if (!parent->hasInheritedPresentationalRole()) {
    if (!getLayoutObject() || !getLayoutObject()->isBoxModelObject())
      return 0;

    LayoutBoxModelObject* cssBox = toLayoutBoxModelObject(getLayoutObject());
    if (!cssBox->isTableCell() && !cssBox->isTableRow())
      return 0;

    if (!isPresentationalInTable(parent, element))
      return 0;
  }
  if (isRequiredOwnedElement(parent, roleValue(), element))
    return parent;
  return 0;
}
