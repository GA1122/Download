bool HTMLFormElement::ValidateInteractively() {
  UseCounter::Count(GetDocument(), WebFeature::kFormValidationStarted);
  for (const auto& element : ListedElements()) {
    if (element->IsFormControlElement())
      ToHTMLFormControlElement(element)->HideVisibleValidationMessage();
  }

  HeapVector<Member<HTMLFormControlElement>> unhandled_invalid_controls;
  if (!CheckInvalidControlsAndCollectUnhandled(
          &unhandled_invalid_controls, kCheckValidityDispatchInvalidEvent))
    return true;
  UseCounter::Count(GetDocument(),
                    WebFeature::kFormValidationAbortedSubmission);

  GetDocument().UpdateStyleAndLayoutIgnorePendingStylesheets();

  for (const auto& unhandled : unhandled_invalid_controls) {
    if (unhandled->IsFocusable()) {
      unhandled->ShowValidationMessage();
      UseCounter::Count(GetDocument(),
                        WebFeature::kFormValidationShowedMessage);
      break;
    }
  }
  if (GetDocument().GetFrame()) {
    for (const auto& unhandled : unhandled_invalid_controls) {
      if (unhandled->IsFocusable())
        continue;
      String message(
          "An invalid form control with name='%name' is not focusable.");
      message.Replace("%name", unhandled->GetName());
      GetDocument().AddConsoleMessage(ConsoleMessage::Create(
          kRenderingMessageSource, kErrorMessageLevel, message));
    }
  }
  return false;
}
