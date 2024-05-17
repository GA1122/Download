void HTMLFormControlElement::didChangeForm() {
  ListedElement::didChangeForm();
  formOwnerSetNeedsValidityCheck();
  if (formOwner() && isConnected() && canBeSuccessfulSubmitButton())
    formOwner()->invalidateDefaultButtonStyle();
}
