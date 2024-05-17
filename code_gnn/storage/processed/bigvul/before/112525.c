PassRefPtr<CustomElementConstructor> Document::registerElement(WebCore::ScriptState* state, const AtomicString& name, const Dictionary& options, ExceptionCode& ec)
{
    if (!isHTMLDocument() && !isXHTMLDocument()) {
        ec = NOT_SUPPORTED_ERR;
        return 0;
    }

    if (!m_registry)
        m_registry = adoptRef(new CustomElementRegistry(this));
    return m_registry->registerElement(state, name, options, ec);
}
