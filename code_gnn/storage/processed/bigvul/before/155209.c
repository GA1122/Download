bool HTMLFormElement::checkInvalidControlsAndCollectUnhandled(
    HeapVector<Member<HTMLFormControlElement>>* unhandledInvalidControls,
    CheckValidityEventBehavior eventBehavior) {
  const FormAssociatedElement::List& associatedElements =
      this->associatedElements();
  HeapVector<Member<FormAssociatedElement>> elements;
  elements.reserveCapacity(associatedElements.size());
  for (unsigned i = 0; i < associatedElements.size(); ++i)
    elements.append(associatedElements[i]);
  int invalidControlsCount = 0;
  for (unsigned i = 0; i < elements.size(); ++i) {
    if (elements[i]->form() == this && elements[i]->isFormControlElement()) {
      HTMLFormControlElement* control =
          toHTMLFormControlElement(elements[i].get());
      if (control->isSubmittableElement() &&
          !control->checkValidity(unhandledInvalidControls, eventBehavior) &&
          control->formOwner() == this) {
        ++invalidControlsCount;
        if (!unhandledInvalidControls &&
            eventBehavior == CheckValidityDispatchNoEvent)
          return true;
      }
    }
  }
  return invalidControlsCount;
}
