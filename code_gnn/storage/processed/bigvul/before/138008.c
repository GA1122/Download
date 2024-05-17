bool AXListBoxOption::isSelected() const {
  return isHTMLOptionElement(getNode()) &&
         toHTMLOptionElement(getNode())->selected();
}
