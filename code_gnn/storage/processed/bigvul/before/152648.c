bool HTMLFormControlElement::isAutofocusable() const {
  return fastHasAttribute(autofocusAttr) && supportsAutofocus();
}
