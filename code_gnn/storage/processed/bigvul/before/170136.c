bool FindFormInputElement(
    const std::vector<blink::WebFormControlElement>& control_elements,
    const FormFieldData& field,
    bool ambiguous_or_empty_names,
    FormInputElementMap* result) {
  bool found_input = false;
  bool is_password_field = IsPasswordField(field);
  bool does_password_field_has_ambigous_or_empty_name =
      ambiguous_or_empty_names && is_password_field;
  bool ambiguous_and_multiple_password_fields_with_autocomplete =
      does_password_field_has_ambigous_or_empty_name &&
      HasPasswordWithAutocompleteAttribute(control_elements);
  base::string16 field_name = FieldName(field, ambiguous_or_empty_names);
  for (const blink::WebFormControlElement& control_element : control_elements) {
    if (!ambiguous_or_empty_names &&
        control_element.nameForAutofill() != field_name) {
      continue;
    }

    if (!control_element.hasHTMLTagName("input"))
      continue;

    const blink::WebInputElement input_element =
        control_element.toConst<blink::WebInputElement>();
    if (input_element.isPasswordField() != is_password_field)
      continue;

    if (ambiguous_and_multiple_password_fields_with_autocomplete &&
        !HasAutocompleteAttributeValue(input_element, "current-password")) {
      continue;
    }

    if (found_input) {
      if (does_password_field_has_ambigous_or_empty_name) {
        if (!form_util::IsWebNodeVisible((*result)[field_name])) {
          (*result)[field_name] = input_element;
        }
        continue;
      }

      found_input = false;
      break;
    }

    (*result)[field_name] = input_element;
    found_input = true;
  }

  if (!found_input) {
    result->clear();
    return false;
  }

  return true;
}
