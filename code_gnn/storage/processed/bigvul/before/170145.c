bool HasPasswordWithAutocompleteAttribute(
    const std::vector<blink::WebFormControlElement>& control_elements) {
  for (const blink::WebFormControlElement& control_element : control_elements) {
    if (!control_element.hasHTMLTagName("input"))
      continue;

    const blink::WebInputElement input_element =
        control_element.toConst<blink::WebInputElement>();
    if (input_element.isPasswordField() &&
        (HasAutocompleteAttributeValue(input_element, "current-password") ||
         HasAutocompleteAttributeValue(input_element, "new-password")))
      return true;
  }

  return false;
}
