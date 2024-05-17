 AXObject* AXObjectCacheImpl::createFromRenderer(LayoutObject* layoutObject) {
  Node* node = layoutObject->node();

  if (nodeHasRole(node, "list") || nodeHasRole(node, "directory") ||
      (nodeHasRole(node, nullAtom) &&
       (isHTMLUListElement(node) || isHTMLOListElement(node) ||
        isHTMLDListElement(node))))
    return AXList::create(layoutObject, *this);

  if (nodeHasRole(node, "grid") || nodeHasRole(node, "treegrid"))
    return AXARIAGrid::create(layoutObject, *this);
  if (nodeHasRole(node, "row"))
    return AXARIAGridRow::create(layoutObject, *this);
  if (nodeHasRole(node, "gridcell") || nodeHasRole(node, "columnheader") ||
      nodeHasRole(node, "rowheader"))
    return AXARIAGridCell::create(layoutObject, *this);

  if (node && node->isMediaControlElement())
    return AccessibilityMediaControl::create(layoutObject, *this);

  if (isHTMLOptionElement(node))
    return AXListBoxOption::create(layoutObject, *this);

  if (isHTMLInputElement(node) &&
      toHTMLInputElement(node)->type() == InputTypeNames::radio)
    return AXRadioInput::create(layoutObject, *this);

  if (layoutObject->isSVGRoot())
    return AXSVGRoot::create(layoutObject, *this);

  if (layoutObject->isBoxModelObject()) {
    LayoutBoxModelObject* cssBox = toLayoutBoxModelObject(layoutObject);
    if (cssBox->isListBox())
      return AXListBox::create(toLayoutListBox(cssBox), *this);
    if (cssBox->isMenuList())
      return AXMenuList::create(toLayoutMenuList(cssBox), *this);

    if (cssBox->isTable())
      return AXTable::create(toLayoutTable(cssBox), *this);
    if (cssBox->isTableRow())
      return AXTableRow::create(toLayoutTableRow(cssBox), *this);
    if (cssBox->isTableCell())
      return AXTableCell::create(toLayoutTableCell(cssBox), *this);

    if (cssBox->isProgress())
      return AXProgressIndicator::create(toLayoutProgress(cssBox), *this);

    if (cssBox->isSlider())
      return AXSlider::create(toLayoutSlider(cssBox), *this);
  }

  return AXLayoutObject::create(layoutObject, *this);
}
