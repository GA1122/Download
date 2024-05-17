bool HTMLFormControlElement::isDisabledFormControl() const {
  if (fastHasAttribute(disabledAttr))
    return true;

  if (m_ancestorDisabledState == AncestorDisabledStateUnknown)
    updateAncestorDisabledState();
  return m_ancestorDisabledState == AncestorDisabledStateDisabled;
}
