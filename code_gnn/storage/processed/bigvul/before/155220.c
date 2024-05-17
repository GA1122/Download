HTMLFormControlElement* HTMLFormElement::findDefaultButton() const {
  for (const auto& element : associatedElements()) {
    if (!element->isFormControlElement())
      continue;
    HTMLFormControlElement* control = toHTMLFormControlElement(element);
    if (control->canBeSuccessfulSubmitButton())
      return control;
  }
  return nullptr;
}
