void AXLayoutObject::loadInlineTextBoxes() {
  if (!getLayoutObject() || !getLayoutObject()->isText())
    return;

  clearChildren();
  addInlineTextBoxChildren(true);
}
