HTMLFormControlElement* HTMLFormElement::FindDefaultButton() const {
  for (const auto& element : ListedElements()) {
    if (!element->IsFormControlElement())
      continue;
    HTMLFormControlElement* control = ToHTMLFormControlElement(element);
    if (control->CanBeSuccessfulSubmitButton())
      return control;
  }
  return nullptr;
}
