ScriptValue Document::registerElement(ScriptState* scriptState, const AtomicString& name, const ElementRegistrationOptions& options, ExceptionState& exceptionState, CustomElement::NameSet validNames)
{
    OriginsUsingFeatures::countMainWorldOnly(scriptState, *this, OriginsUsingFeatures::Feature::DocumentRegisterElement);

    if (!registrationContext()) {
        exceptionState.throwDOMException(NotSupportedError, "No element registration context is available.");
        return ScriptValue();
    }

    CustomElementConstructorBuilder constructorBuilder(scriptState, options);
    registrationContext()->registerElement(this, &constructorBuilder, name, validNames, exceptionState);
    return constructorBuilder.bindingsReturnValue();
}
