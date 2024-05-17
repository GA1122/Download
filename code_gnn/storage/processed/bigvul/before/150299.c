void PasswordAutofillAgent::FocusedNodeHasChanged(const blink::WebNode& node) {
  DCHECK(!node.IsNull());
  focused_input_element_.Reset();
  if (!node.IsElementNode()) {
    focus_state_notifier_.FocusedInputChanged(FocusedFieldType::kUnknown);
    return;
  }

  auto element = node.ToConst<WebElement>();
  if (element.IsFormControlElement() &&
      form_util::IsTextAreaElement(element.ToConst<WebFormControlElement>())) {
    focus_state_notifier_.FocusedInputChanged(
        FocusedFieldType::kFillableTextArea);
    return;
  }

  auto* input_element = ToWebInputElement(&element);
  if (!input_element) {
    focus_state_notifier_.FocusedInputChanged(
        FocusedFieldType::kUnfillableElement);
    return;
  }

  auto focused_field_type = FocusedFieldType::kUnfillableElement;
  if (input_element->IsTextField() && IsElementEditable(*input_element)) {
    focused_input_element_ = *input_element;

    WebString type = input_element->GetAttribute("type");
    if (!type.IsNull() && type == "search")
      focused_field_type = FocusedFieldType::kFillableSearchField;
    else if (input_element->IsPasswordFieldForAutofill())
      focused_field_type = FocusedFieldType::kFillablePasswordField;
    else if (base::Contains(web_input_to_password_info_, *input_element))
      focused_field_type = FocusedFieldType::kFillableUsernameField;
    else
      focused_field_type = FocusedFieldType::kFillableNonSearchField;
  }

  focus_state_notifier_.FocusedInputChanged(focused_field_type);
  field_data_manager_.UpdateFieldDataMapWithNullValue(
      *input_element, FieldPropertiesFlags::HAD_FOCUS);
}
