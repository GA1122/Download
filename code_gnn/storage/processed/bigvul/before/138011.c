void AXListBoxOption::setSelected(bool selected) {
  HTMLSelectElement* selectElement = listBoxOptionParentNode();
  if (!selectElement)
    return;

  if (!canSetSelectedAttribute())
    return;

  bool isOptionSelected = isSelected();
  if ((isOptionSelected && selected) || (!isOptionSelected && !selected))
    return;

  selectElement->selectOptionByAccessKey(toHTMLOptionElement(getNode()));
}
