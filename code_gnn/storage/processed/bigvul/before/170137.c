bool FindFormInputElements(
    const std::vector<blink::WebFormControlElement>& control_elements,
    const PasswordFormFillData& data,
    bool ambiguous_or_empty_names,
    FormInputElementMap* result) {
  return FindFormInputElement(control_elements, data.password_field,
                              ambiguous_or_empty_names, result) &&
         (!FillDataContainsFillableUsername(data) ||
          FindFormInputElement(control_elements, data.username_field,
                               ambiguous_or_empty_names, result));
}
