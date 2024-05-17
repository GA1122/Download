Element* Document::createElementNS(const AtomicString& namespace_uri,
                                   const AtomicString& qualified_name,
                                   ExceptionState& exception_state) {
  QualifiedName q_name(
      CreateQualifiedName(namespace_uri, qualified_name, exception_state));
  if (q_name == QualifiedName::Null())
    return nullptr;

  if (CustomElement::ShouldCreateCustomElement(q_name))
    return CustomElement::CreateCustomElementSync(*this, q_name);
  return createElement(q_name, kCreatedByCreateElement);
}
