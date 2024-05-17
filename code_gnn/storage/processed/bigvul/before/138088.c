 bool AXNodeObject::isTextControl() const {
  if (hasContentEditableAttributeSet())
    return true;

  switch (roleValue()) {
    case TextFieldRole:
    case ComboBoxRole:
    case SearchBoxRole:
    case SpinButtonRole:
      return true;
    default:
      return false;
  }
}
