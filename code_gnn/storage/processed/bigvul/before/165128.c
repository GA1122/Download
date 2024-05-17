bool HTMLFormElement::CheckInvalidControlsAndCollectUnhandled(
    HeapVector<Member<HTMLFormControlElement>>* unhandled_invalid_controls,
    CheckValidityEventBehavior event_behavior) {
  const ListedElement::List& listed_elements = ListedElements();
  HeapVector<Member<ListedElement>> elements;
  elements.ReserveCapacity(listed_elements.size());
  for (const auto& element : listed_elements)
    elements.push_back(element);
  int invalid_controls_count = 0;
  for (const auto& element : elements) {
    if (element->Form() == this && element->IsFormControlElement()) {
      HTMLFormControlElement* control = ToHTMLFormControlElement(element);
      if (control->IsSubmittableElement() &&
          !control->checkValidity(unhandled_invalid_controls, event_behavior) &&
          control->formOwner() == this) {
        ++invalid_controls_count;
        if (!unhandled_invalid_controls &&
            event_behavior == kCheckValidityDispatchNoEvent)
          return true;
      }
    }
  }
  return invalid_controls_count;
}
