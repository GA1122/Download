ScriptValue Document::registerElement(ScriptState* scriptState, const AtomicString& name, const ElementRegistrationOptions& options, ExceptionState& exceptionState, V0CustomElement::NameSet validNames)
{
    HostsUsingFeatures::countMainWorldOnly(scriptState, *this, HostsUsingFeatures::Feature::DocumentRegisterElement);

    if (!registrationContext()) {
        exceptionState.throwDOMException(NotSupportedError, "No element registration context is available.");
        return ScriptValue();
    }

    V0CustomElementConstructorBuilder constructorBuilder(scriptState, options);
    registrationContext()->registerElement(this, &constructorBuilder, name, validNames, exceptionState);
    return constructorBuilder.bindingsReturnValue();
}
