void HTMLFormElement::invalidateDefaultButtonStyle() const {
  for (const auto& control : associatedElements()) {
    if (!control->isFormControlElement())
      continue;
    if (toHTMLFormControlElement(control)->canBeSuccessfulSubmitButton())
      toHTMLFormControlElement(control)->pseudoStateChanged(
          CSSSelector::PseudoDefault);
  }
}
