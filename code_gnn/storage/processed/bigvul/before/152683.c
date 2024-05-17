bool HTMLFormControlElement::willValidate() const {
  if (!m_willValidateInitialized || m_dataListAncestorState == Unknown) {
    const_cast<HTMLFormControlElement*>(this)->setNeedsWillValidateCheck();
  } else {
    DCHECK_EQ(m_willValidate, recalcWillValidate());
  }
  return m_willValidate;
}
