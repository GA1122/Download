Element* Document::CreateElementForBinding(
    const AtomicString& local_name,
    const StringOrElementCreationOptions& string_or_options,
    ExceptionState& exception_state) {
  if (string_or_options.IsNull()) {
    return CreateElementForBinding(local_name, exception_state);
  }

  if (!IsValidElementName(this, local_name)) {
    exception_state.ThrowDOMException(
        DOMExceptionCode::kInvalidCharacterError,
        "The tag name provided ('" + local_name + "') is not a valid name.");
    return nullptr;
  }

  const AtomicString& converted_local_name = ConvertLocalName(local_name);
  QualifiedName q_name(g_null_atom, converted_local_name,
                       IsXHTMLDocument() || IsHTMLDocument()
                           ? html_names::xhtmlNamespaceURI
                           : g_null_atom);

  bool is_v1 =
      string_or_options.IsElementCreationOptions() || !RegistrationContext();
  bool create_v1_builtin = string_or_options.IsElementCreationOptions();
  bool should_create_builtin =
      create_v1_builtin || string_or_options.IsString();

  const AtomicString& is = GetTypeExtension(this, string_or_options);

  Element* element =
      CreateElement(q_name,
                    is_v1 ? CreateElementFlags::ByCreateElementV1()
                          : CreateElementFlags::ByCreateElementV0(),
                    should_create_builtin ? is : g_null_atom);

  if (!is_v1 && !is.IsEmpty())
    element->setAttribute(html_names::kIsAttr, is);

  return element;
}
