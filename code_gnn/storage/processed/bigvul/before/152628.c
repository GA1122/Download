bool HTMLFormControlElement::checkValidity(
    HeapVector<Member<HTMLFormControlElement>>* unhandledInvalidControls,
    CheckValidityEventBehavior eventBehavior) {
  if (!willValidate())
    return true;
  if (isValidElement())
    return true;
  if (eventBehavior != CheckValidityDispatchInvalidEvent)
    return false;
  Document* originalDocument = &document();
  DispatchEventResult dispatchResult =
      dispatchEvent(Event::createCancelable(EventTypeNames::invalid));
  if (dispatchResult == DispatchEventResult::NotCanceled &&
      unhandledInvalidControls && isConnected() &&
      originalDocument == document())
    unhandledInvalidControls->append(this);
  return false;
}
