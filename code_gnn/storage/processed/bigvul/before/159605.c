Element* Document::createElementNS(const AtomicString& namespace_uri,
                                   const AtomicString& qualified_name,
                                   const StringOrDictionary& string_or_options,
                                   ExceptionState& exception_state) {
  QualifiedName q_name(
      CreateQualifiedName(namespace_uri, qualified_name, exception_state));
  if (q_name == QualifiedName::Null())
    return nullptr;

  bool is_v1 = string_or_options.IsDictionary() || !RegistrationContext();
  bool create_v1_builtin =
      string_or_options.IsDictionary() &&
      RuntimeEnabledFeatures::CustomElementsBuiltinEnabled();
  bool should_create_builtin =
      create_v1_builtin || string_or_options.IsString();

  const AtomicString& is =
      AtomicString(GetTypeExtension(this, string_or_options, exception_state));
  const AtomicString& name = should_create_builtin ? is : qualified_name;

  if (!IsValidElementName(this, qualified_name)) {
    exception_state.ThrowDOMException(
        kInvalidCharacterError, "The tag name provided ('" + qualified_name +
                                    "') is not a valid name.");
    return nullptr;
  }

  CustomElementDefinition* definition = nullptr;
  if (is_v1) {
    const CustomElementDescriptor desc =
        RuntimeEnabledFeatures::CustomElementsBuiltinEnabled()
            ? CustomElementDescriptor(name, qualified_name)
            : CustomElementDescriptor(qualified_name, qualified_name);
    if (CustomElementRegistry* registry = CustomElement::Registry(*this))
      definition = registry->DefinitionFor(desc);

    if (!definition && create_v1_builtin) {
      exception_state.ThrowDOMException(kNotFoundError,
                                        "Custom element definition not found.");
      return nullptr;
    }
  }

  Element* element;

  if (CustomElement::ShouldCreateCustomElement(q_name) || create_v1_builtin) {
    element = CustomElement::CreateCustomElementSync(*this, q_name, definition);
  } else if (V0CustomElement::IsValidName(q_name.LocalName()) &&
             RegistrationContext()) {
    element = RegistrationContext()->CreateCustomTagElement(*this, q_name);
  } else {
    element = createElement(q_name, kCreatedByCreateElement);
  }

  if (!is.IsEmpty()) {
    if (element->GetCustomElementState() != CustomElementState::kCustom) {
      V0CustomElementRegistrationContext::SetIsAttributeAndTypeExtension(
          element, is);
    } else if (string_or_options.IsDictionary()) {
      element->setAttribute(HTMLNames::isAttr, is);
    }
  }

  return element;
}
