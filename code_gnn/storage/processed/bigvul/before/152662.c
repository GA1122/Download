bool HTMLFormControlElement::reportValidity() {
  HeapVector<Member<HTMLFormControlElement>> unhandledInvalidControls;
  bool isValid = checkValidity(&unhandledInvalidControls,
                               CheckValidityDispatchInvalidEvent);
  if (isValid || unhandledInvalidControls.isEmpty())
    return isValid;
  DCHECK_EQ(unhandledInvalidControls.size(), 1u);
  DCHECK_EQ(unhandledInvalidControls[0].get(), this);
  document().updateStyleAndLayoutIgnorePendingStylesheets();
  if (isFocusable()) {
    showValidationMessage();
    return false;
  }
  if (document().frame()) {
    String message(
        "An invalid form control with name='%name' is not focusable.");
    message.replace("%name", name());
    document().addConsoleMessage(ConsoleMessage::create(
        RenderingMessageSource, ErrorMessageLevel, message));
  }
  return false;
}
