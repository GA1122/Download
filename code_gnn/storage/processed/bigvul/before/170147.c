bool IsElementEditable(const blink::WebInputElement& element) {
  return element.isEnabled() && !element.isReadOnly();
}
