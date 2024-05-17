bool AXListBoxOption::canSetSelectedAttribute() const {
  if (!isHTMLOptionElement(getNode()))
    return false;

  if (toHTMLOptionElement(getNode())->isDisabledFormControl())
    return false;

  HTMLSelectElement* selectElement = listBoxOptionParentNode();
  if (selectElement && selectElement->isDisabledFormControl())
    return false;

  return true;
}
