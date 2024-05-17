void AXLayoutObject::setValue(const String& string) {
  if (!getNode() || !getNode()->isElementNode())
    return;
  if (!m_layoutObject || !m_layoutObject->isBoxModelObject())
    return;

  LayoutBoxModelObject* layoutObject = toLayoutBoxModelObject(m_layoutObject);
  if (layoutObject->isTextField() && isHTMLInputElement(*getNode()))
    toHTMLInputElement(*getNode())
        .setValue(string, DispatchInputAndChangeEvent);
  else if (layoutObject->isTextArea() && isHTMLTextAreaElement(*getNode()))
    toHTMLTextAreaElement(*getNode())
        .setValue(string, DispatchInputAndChangeEvent);
}
