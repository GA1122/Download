bool AXObject::isClickable() const {
  switch (roleValue()) {
    case ButtonRole:
    case CheckBoxRole:
    case ColorWellRole:
    case ComboBoxRole:
    case ImageMapLinkRole:
    case LinkRole:
    case ListBoxOptionRole:
    case MenuButtonRole:
    case PopUpButtonRole:
    case RadioButtonRole:
    case SpinButtonRole:
    case TabRole:
    case TextFieldRole:
    case ToggleButtonRole:
      return true;
    default:
      return false;
  }
}
