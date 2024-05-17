WebInputElement FindUsernameElementPrecedingPasswordElement(
    WebLocalFrame* frame,
    const WebInputElement& password_element) {
  DCHECK(!password_element.IsNull());

  std::vector<WebFormControlElement> elements;
  if (password_element.Form().IsNull()) {
    elements = form_util::GetUnownedAutofillableFormFieldElements(
        frame->GetDocument().All(), nullptr);
  } else {
    WebVector<WebFormControlElement> web_control_elements;
    password_element.Form().GetFormControlElements(web_control_elements);
    elements.assign(web_control_elements.begin(), web_control_elements.end());
  }

  auto iter = std::find(elements.begin(), elements.end(), password_element);
  if (iter == elements.end())
    return WebInputElement();

  for (auto begin = elements.begin(); iter != begin;) {
    --iter;
    const WebInputElement* input = ToWebInputElement(&*iter);
    if (input && input->IsTextField() && !input->IsPasswordFieldForAutofill() &&
        IsElementEditable(*input) && form_util::IsWebElementVisible(*input)) {
      return *input;
    }
  }

  return WebInputElement();
}
