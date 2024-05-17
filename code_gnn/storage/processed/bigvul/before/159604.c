Element* Document::createElement(const AtomicString& local_name,
                                 const StringOrDictionary& string_or_options,
                                 ExceptionState& exception_state) {
  if (!IsValidElementName(this, local_name)) {
    exception_state.ThrowDOMException(
        kInvalidCharacterError,
        "The tag name provided ('" + local_name + "') is not a valid name.");
    return nullptr;
  }

  const AtomicString& converted_local_name = ConvertLocalName(local_name);

  bool is_v1 = string_or_options.IsDictionary() || !RegistrationContext();
  bool create_v1_builtin =
      string_or_options.IsDictionary() &&
      RuntimeEnabledFeatures::CustomElementsBuiltinEnabled();
  bool should_create_builtin =
      create_v1_builtin || string_or_options.IsString();

  const AtomicString& is =
      AtomicString(GetTypeExtension(this, string_or_options, exception_state));
  const AtomicString& name = should_create_builtin ? is : converted_local_name;

  CustomElementDefinition* definition = nullptr;
  if (is_v1) {
    const CustomElementDescriptor desc =
        RuntimeEnabledFeatures::CustomElementsBuiltinEnabled()
            ? CustomElementDescriptor(name, converted_local_name)
            : CustomElementDescriptor(converted_local_name,
                                      converted_local_name);
    if (CustomElementRegistry* registry = CustomElement::Registry(*this))
      definition = registry->DefinitionFor(desc);

    if (!definition && create_v1_builtin) {
      exception_state.ThrowDOMException(kNotFoundError,
                                        "Custom element definition not found.");
      return nullptr;
    }
  }

  Element* element;

  if (definition) {
    element = CustomElement::CreateCustomElementSync(
        *this, converted_local_name, definition);
  } else if (V0CustomElement::IsValidName(local_name) &&
             RegistrationContext()) {
    element = RegistrationContext()->CreateCustomTagElement(
        *this,
        QualifiedName(g_null_atom, converted_local_name, xhtmlNamespaceURI));
  } else {
    element = createElement(local_name, exception_state);
    if (exception_state.HadException())
      return nullptr;
  }

  if (!is.IsEmpty()) {
    if (string_or_options.IsString()) {
      V0CustomElementRegistrationContext::SetIsAttributeAndTypeExtension(
          element, is);
    } else if (string_or_options.IsDictionary()) {
      element->setAttribute(HTMLNames::isAttr, is);
    }
  }

  return element;
}
