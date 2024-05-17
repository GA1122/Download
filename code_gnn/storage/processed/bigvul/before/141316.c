Element* Document::createElementNS(
    const AtomicString& namespace_uri,
    const AtomicString& qualified_name,
    const StringOrElementCreationOptions& string_or_options,
    ExceptionState& exception_state) {
  if (string_or_options.IsNull())
    return createElementNS(namespace_uri, qualified_name, exception_state);

  QualifiedName q_name(
      CreateQualifiedName(namespace_uri, qualified_name, exception_state));
  if (q_name == QualifiedName::Null())
    return nullptr;

  bool is_v1 =
      string_or_options.IsElementCreationOptions() || !RegistrationContext();
  bool create_v1_builtin = string_or_options.IsElementCreationOptions();
  bool should_create_builtin =
      create_v1_builtin || string_or_options.IsString();

  const AtomicString& is = GetTypeExtension(this, string_or_options);

  if (!IsValidElementName(this, qualified_name)) {
    exception_state.ThrowDOMException(DOMExceptionCode::kInvalidCharacterError,
                                      "The tag name provided ('" +
                                          qualified_name +
                                          "') is not a valid name.");
    return nullptr;
  }

  Element* element =
      CreateElement(q_name,
                    is_v1 ? CreateElementFlags::ByCreateElementV1()
                          : CreateElementFlags::ByCreateElementV0(),
                    should_create_builtin ? is : g_null_atom);

  if (!is_v1 && !is.IsEmpty())
    element->setAttribute(html_names::kIsAttr, is);

  return element;
}
