WebInputElement ConvertToWebInput(const WebFormControlElement& element) {
  if (element.IsNull())
    return WebInputElement();
  const WebInputElement* input = ToWebInputElement(&element);
  return input ? *input : WebInputElement();
}
