bool AXObject::supportsActiveDescendant() const {
  switch (ariaRoleAttribute()) {
    case ComboBoxRole:
    case GridRole:
    case GroupRole:
    case ListBoxRole:
    case MenuRole:
    case MenuBarRole:
    case RadioGroupRole:
    case RowRole:
    case SearchBoxRole:
    case TabListRole:
    case TextFieldRole:
    case ToolbarRole:
    case TreeRole:
    case TreeGridRole:
      return true;
    default:
      return false;
  }
}
