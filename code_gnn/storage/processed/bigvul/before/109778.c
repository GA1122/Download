ScriptValue Document::registerElement(WebCore::ScriptState* state, const AtomicString& name, ExceptionState& es)
{
    return registerElement(state, name, Dictionary(), es);
}
