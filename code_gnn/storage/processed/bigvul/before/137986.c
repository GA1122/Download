String AXLayoutObject::stringValue() const {
  if (!m_layoutObject)
    return String();

  LayoutBoxModelObject* cssBox = getLayoutBoxModelObject();

  if (cssBox && cssBox->isMenuList()) {
    HTMLSelectElement* selectElement =
        toHTMLSelectElement(m_layoutObject->node());
    int selectedIndex = selectElement->selectedIndex();
    const HeapVector<Member<HTMLElement>>& listItems =
        selectElement->listItems();
    if (selectedIndex >= 0 &&
        static_cast<size_t>(selectedIndex) < listItems.size()) {
      const AtomicString& overriddenDescription =
          listItems[selectedIndex]->fastGetAttribute(aria_labelAttr);
      if (!overriddenDescription.isNull())
        return overriddenDescription;
    }
    return toLayoutMenuList(m_layoutObject)->text();
  }

  if (isWebArea()) {
    if (m_layoutObject->frame())
      return String();

    ASSERT_NOT_REACHED();
  }

  if (isTextControl())
    return text();

  if (m_layoutObject->isFileUploadControl())
    return toLayoutFileUploadControl(m_layoutObject)->fileTextValue();

  if (getNode() && isHTMLInputElement(getNode())) {
    HTMLInputElement* input = toHTMLInputElement(getNode());
    if (input->type() != InputTypeNames::checkbox &&
        input->type() != InputTypeNames::radio)
      return input->value();
  }

  return String();
}
