void HTMLFormControlElement::setNeedsValidityCheck()
{
    if (!m_validityIsDirty) {
        m_validityIsDirty = true;
        formOwnerSetNeedsValidityCheck();
        fieldSetAncestorsSetNeedsValidityCheck(parentNode());
        pseudoStateChanged(CSSSelector::PseudoValid);
        pseudoStateChanged(CSSSelector::PseudoInvalid);
    }

    if (isValidationMessageVisible()) {
        updateVisibleValidationMessage();
    }
}
