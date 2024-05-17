Element* Document::createElementNS(const AtomicString& namespace_uri,
                                   const AtomicString& qualified_name,
                                   ExceptionState& exception_state) {
  QualifiedName q_name(
      CreateQualifiedName(namespace_uri, qualified_name, exception_state));
  if (q_name == QualifiedName::Null())
    return nullptr;

  CreateElementFlags flags = CreateElementFlags::ByCreateElement();
  if (CustomElement::ShouldCreateCustomElement(q_name))
    return CustomElement::CreateCustomElement(*this, q_name, flags);
  if (RegistrationContext() && V0CustomElement::IsValidName(q_name.LocalName()))
    return RegistrationContext()->CreateCustomTagElement(*this, q_name);
  return CreateRawElement(q_name, flags);
}
