HTMLFormControlElement::HTMLFormControlElement(const QualifiedName& tagName,
                                               Document& document)
    : LabelableElement(tagName, document),
      m_ancestorDisabledState(AncestorDisabledStateUnknown),
      m_dataListAncestorState(Unknown),
      m_isAutofilled(false),
      m_hasValidationMessage(false),
      m_willValidateInitialized(false),
      m_willValidate(true),
      m_isValid(true),
      m_validityIsDirty(false),
      m_wasFocusedByMouse(false) {
  setHasCustomStyleCallbacks();
}
