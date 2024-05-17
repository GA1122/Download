Element* Document::CreateElementForBinding(const AtomicString& name,
                                           ExceptionState& exception_state) {
  if (!IsValidElementName(this, name)) {
    exception_state.ThrowDOMException(
        DOMExceptionCode::kInvalidCharacterError,
        "The tag name provided ('" + name + "') is not a valid name.");
    return nullptr;
  }

  if (IsXHTMLDocument() || IsHTMLDocument()) {
    AtomicString local_name = ConvertLocalName(name);
    if (CustomElement::ShouldCreateCustomElement(local_name)) {
      return CustomElement::CreateCustomElement(
          *this,
          QualifiedName(g_null_atom, local_name, html_names::xhtmlNamespaceURI),
          CreateElementFlags::ByCreateElement());
    }
    if (auto* element = HTMLElementFactory::Create(
            local_name, *this, CreateElementFlags::ByCreateElement()))
      return element;
    QualifiedName q_name(g_null_atom, local_name,
                         html_names::xhtmlNamespaceURI);
    if (RegistrationContext() && V0CustomElement::IsValidName(local_name))
      return RegistrationContext()->CreateCustomTagElement(*this, q_name);
    return HTMLUnknownElement::Create(q_name, *this);
  }
  return Element::Create(QualifiedName(g_null_atom, name, g_null_atom), this);
}
