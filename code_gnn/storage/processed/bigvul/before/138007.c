bool AXListBoxOption::isParentPresentationalRole() const {
  AXObject* parent = parentObject();
  if (!parent)
    return false;

  LayoutObject* layoutObject = parent->getLayoutObject();
  if (!layoutObject)
    return false;

  if (layoutObject->isListBox() && parent->hasInheritedPresentationalRole())
    return true;

  return false;
}
