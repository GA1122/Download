void HTMLFormControlElement::setNeedsValidityCheck() {
  if (!m_validityIsDirty) {
    m_validityIsDirty = true;
    formOwnerSetNeedsValidityCheck();
    fieldSetAncestorsSetNeedsValidityCheck(parentNode());
    pseudoStateChanged(CSSSelector::PseudoValid);
    pseudoStateChanged(CSSSelector::PseudoInvalid);
    pseudoStateChanged(CSSSelector::PseudoInRange);
    pseudoStateChanged(CSSSelector::PseudoOutOfRange);
  }

  if (isValidationMessageVisible()) {
    updateVisibleValidationMessage();
  }
}
