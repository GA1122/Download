ScriptValue Document::registerElement(ScriptState* script_state,
                                      const AtomicString& name,
                                      const ElementRegistrationOptions* options,
                                      ExceptionState& exception_state) {
  if (!RegistrationContext()) {
    exception_state.ThrowDOMException(
        DOMExceptionCode::kNotSupportedError,
        "No element registration context is available.");
    return ScriptValue();
  }

  if (name == "dom-module")
    UseCounter::Count(*this, WebFeature::kPolymerV1Detected);

  V0CustomElementConstructorBuilder constructor_builder(script_state, options);
  RegistrationContext()->RegisterElement(this, &constructor_builder, name,
                                         exception_state);
  return constructor_builder.BindingsReturnValue();
}
