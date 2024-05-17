AccessibilityButtonState AXNodeObject::checkboxOrRadioValue() const {
  if (isNativeCheckboxInMixedState())
    return ButtonStateMixed;

  if (isNativeCheckboxOrRadio())
    return isChecked() ? ButtonStateOn : ButtonStateOff;

  return AXObject::checkboxOrRadioValue();
}
