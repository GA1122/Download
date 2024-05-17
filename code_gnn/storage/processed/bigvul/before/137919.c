bool AXLayoutObject::ariaRoleHasPresentationalChildren() const {
  switch (m_ariaRole) {
    case ButtonRole:
    case SliderRole:
    case ImageRole:
    case ProgressIndicatorRole:
    case SpinButtonRole:
      return true;
    default:
      return false;
  }
}
