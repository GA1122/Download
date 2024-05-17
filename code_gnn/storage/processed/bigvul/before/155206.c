void HTMLFormElement::associate(FormAssociatedElement& e) {
  m_associatedElementsAreDirty = true;
  m_associatedElements.clear();
  if (toHTMLElement(e).fastHasAttribute(formAttr))
    m_hasElementsAssociatedByFormAttribute = true;
}
