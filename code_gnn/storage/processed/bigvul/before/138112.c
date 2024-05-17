String AXNodeObject::stringValue() const {
  Node* node = this->getNode();
  if (!node)
    return String();

  if (isHTMLSelectElement(*node)) {
    HTMLSelectElement& selectElement = toHTMLSelectElement(*node);
    int selectedIndex = selectElement.selectedIndex();
    const HeapVector<Member<HTMLElement>>& listItems =
        selectElement.listItems();
    if (selectedIndex >= 0 &&
        static_cast<size_t>(selectedIndex) < listItems.size()) {
      const AtomicString& overriddenDescription =
          listItems[selectedIndex]->fastGetAttribute(aria_labelAttr);
      if (!overriddenDescription.isNull())
        return overriddenDescription;
    }
    if (!selectElement.isMultiple())
      return selectElement.value();
    return String();
  }

  if (isNativeTextControl())
    return text();

  if (isHTMLInputElement(node)) {
    HTMLInputElement* input = toHTMLInputElement(node);
    if (input->type() != InputTypeNames::checkbox &&
        input->type() != InputTypeNames::radio)
      return input->value();
  }

  return String();
}
