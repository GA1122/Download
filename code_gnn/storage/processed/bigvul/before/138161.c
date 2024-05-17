bool AXObject::hasInteractiveARIAAttribute(const Element& element) {
  for (size_t i = 0; i < WTF_ARRAY_LENGTH(ariaInteractiveWidgetAttributes);
       ++i) {
    const char* attribute = ariaInteractiveWidgetAttributes[i];
    if (element.hasAttribute(attribute)) {
      return true;
    }
  }
  return false;
}
