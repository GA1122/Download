bool HTMLFormElement::validateInteractively() {
  UseCounter::count(document(), UseCounter::FormValidationStarted);
  const FormAssociatedElement::List& elements = associatedElements();
  for (unsigned i = 0; i < elements.size(); ++i) {
    if (elements[i]->isFormControlElement())
      toHTMLFormControlElement(elements[i])->hideVisibleValidationMessage();
  }

  HeapVector<Member<HTMLFormControlElement>> unhandledInvalidControls;
  if (!checkInvalidControlsAndCollectUnhandled(
          &unhandledInvalidControls, CheckValidityDispatchInvalidEvent))
    return true;
  UseCounter::count(document(), UseCounter::FormValidationAbortedSubmission);

  document().updateStyleAndLayoutIgnorePendingStylesheets();

  for (unsigned i = 0; i < unhandledInvalidControls.size(); ++i) {
    HTMLFormControlElement* unhandled = unhandledInvalidControls[i].get();
    if (unhandled->isFocusable()) {
      unhandled->showValidationMessage();
      UseCounter::count(document(), UseCounter::FormValidationShowedMessage);
      break;
    }
  }
  if (document().frame()) {
    for (unsigned i = 0; i < unhandledInvalidControls.size(); ++i) {
      HTMLFormControlElement* unhandled = unhandledInvalidControls[i].get();
      if (unhandled->isFocusable())
        continue;
      String message(
          "An invalid form control with name='%name' is not focusable.");
      message.replace("%name", unhandled->name());
      document().addConsoleMessage(ConsoleMessage::create(
          RenderingMessageSource, ErrorMessageLevel, message));
    }
  }
  return false;
}
