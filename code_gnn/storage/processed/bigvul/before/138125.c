AXSupportedAction AXObject::action() const {
  if (!actionElement())
    return AXSupportedAction::None;

  switch (roleValue()) {
    case ButtonRole:
    case ToggleButtonRole:
      return AXSupportedAction::Press;
    case TextFieldRole:
      return AXSupportedAction::Activate;
    case RadioButtonRole:
      return AXSupportedAction::Select;
    case CheckBoxRole:
    case SwitchRole:
      return isChecked() ? AXSupportedAction::Check
                         : AXSupportedAction::Uncheck;
    case LinkRole:
      return AXSupportedAction::Jump;
    case PopUpButtonRole:
      return AXSupportedAction::Open;
    default:
      return AXSupportedAction::Click;
  }
}
