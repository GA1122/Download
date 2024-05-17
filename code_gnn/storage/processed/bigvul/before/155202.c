HTMLFormElement::HTMLFormElement(Document& document)
    : HTMLElement(formTag, document),
      m_associatedElementsAreDirty(false),
      m_imageElementsAreDirty(false),
      m_hasElementsAssociatedByParser(false),
      m_hasElementsAssociatedByFormAttribute(false),
      m_didFinishParsingChildren(false),
      m_isInResetFunction(false),
      m_wasDemoted(false) {}
