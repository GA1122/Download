void HTMLFormControlElement::formOwnerSetNeedsValidityCheck() {
  if (HTMLFormElement* form = formOwner()) {
    form->pseudoStateChanged(CSSSelector::PseudoValid);
    form->pseudoStateChanged(CSSSelector::PseudoInvalid);
  }
}
