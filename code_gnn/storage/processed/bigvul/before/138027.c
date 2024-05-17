bool AXNodeObject::canHaveChildren() const {
  if (!getNode() && !isAXLayoutObject())
    return false;

  if (getNode() && isHTMLMapElement(getNode()))
    return false;

  AccessibilityRole role = roleValue();

  if (isPresentational())
    role = nativeAccessibilityRoleIgnoringAria();

  switch (role) {
    case ImageRole:
    case ButtonRole:
    case PopUpButtonRole:
    case CheckBoxRole:
    case RadioButtonRole:
    case SwitchRole:
    case TabRole:
    case ToggleButtonRole:
    case ListBoxOptionRole:
    case ScrollBarRole:
      return false;
    case StaticTextRole:
      if (!axObjectCache().inlineTextBoxAccessibilityEnabled())
        return false;
    default:
      return true;
  }
}
