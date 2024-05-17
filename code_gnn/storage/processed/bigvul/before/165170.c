void HTMLFormElement::reset() {
  LocalFrame* frame = GetDocument().GetFrame();
  if (is_in_reset_function_ || !frame)
    return;

  is_in_reset_function_ = true;

  if (DispatchEvent(Event::CreateCancelableBubble(EventTypeNames::reset)) !=
      DispatchEventResult::kNotCanceled) {
    is_in_reset_function_ = false;
    return;
  }

  ListedElement::List elements(ListedElements());
  for (const auto& element : elements) {
    if (element->IsFormControlElement())
      ToHTMLFormControlElement(element)->Reset();
  }

  is_in_reset_function_ = false;
}
