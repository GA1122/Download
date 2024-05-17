bool AXObject::isARIATextControl() const {
  return ariaRoleAttribute() == TextFieldRole ||
         ariaRoleAttribute() == SearchBoxRole ||
         ariaRoleAttribute() == ComboBoxRole;
}
