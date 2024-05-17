ScriptValue Document::registerElement(WebCore::ScriptState* state, const AtomicString& name, const Dictionary& options, ExceptionState& es, CustomElement::NameSet validNames)
{
    if (!registrationContext()) {
        es.throwUninformativeAndGenericDOMException(NotSupportedError);
        return ScriptValue();
    }

    CustomElementConstructorBuilder constructorBuilder(state, &options);
    registrationContext()->registerElement(this, &constructorBuilder, name, validNames, es);
    return constructorBuilder.bindingsReturnValue();
}
