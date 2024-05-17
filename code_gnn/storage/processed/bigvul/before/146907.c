ScriptValue Document::registerElement(ScriptState* script_state,
                                      const AtomicString& name,
                                      const ElementRegistrationOptions& options,
                                      ExceptionState& exception_state,
                                      V0CustomElement::NameSet valid_names) {
  HostsUsingFeatures::CountMainWorldOnly(
      script_state, *this,
      HostsUsingFeatures::Feature::kDocumentRegisterElement);

  if (!RegistrationContext()) {
    exception_state.ThrowDOMException(
        kNotSupportedError, "No element registration context is available.");
    return ScriptValue();
  }

  V0CustomElementConstructorBuilder constructor_builder(script_state, options);
  RegistrationContext()->RegisterElement(this, &constructor_builder, name,
                                         valid_names, exception_state);
  return constructor_builder.BindingsReturnValue();
}
