bool AXObject::isMenuRelated() const {
  switch (roleValue()) {
    case MenuRole:
    case MenuBarRole:
    case MenuButtonRole:
    case MenuItemRole:
    case MenuItemCheckBoxRole:
    case MenuItemRadioRole:
      return true;
    default:
      return false;
  }
}
