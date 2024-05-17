Element* Document::createElement(const AtomicString& name,
                                 ExceptionState& exception_state) {
  if (!IsValidElementName(this, name)) {
    exception_state.ThrowDOMException(
        kInvalidCharacterError,
        "The tag name provided ('" + name + "') is not a valid name.");
    return nullptr;
  }

  if (IsXHTMLDocument() || IsHTMLDocument()) {
    AtomicString local_name = ConvertLocalName(name);
    if (CustomElement::ShouldCreateCustomElement(local_name)) {
      return CustomElement::CreateCustomElementSync(
          *this,
          QualifiedName(g_null_atom, local_name, HTMLNames::xhtmlNamespaceURI));
    }
    return HTMLElementFactory::createHTMLElement(local_name, *this,
                                                 kCreatedByCreateElement);
  }
  return Element::Create(QualifiedName(g_null_atom, name, g_null_atom), this);
}
