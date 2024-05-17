HTMLSelectElement* AXListBoxOption::listBoxOptionParentNode() const {
  if (!getNode())
    return 0;

  if (isHTMLOptionElement(getNode()))
    return toHTMLOptionElement(getNode())->ownerSelectElement();

  return 0;
}
