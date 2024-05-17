bool AXListBoxOption::isSelectedOptionActive() const {
  HTMLSelectElement* listBoxParentNode = listBoxOptionParentNode();
  if (!listBoxParentNode)
    return false;

  return listBoxParentNode->activeSelectionEnd() == getNode();
}
