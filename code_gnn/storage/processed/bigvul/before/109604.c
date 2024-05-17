PassRefPtr<Element> Document::createElement(const AtomicString& name, ExceptionState& es)
{
    if (!isValidName(name)) {
        es.throwUninformativeAndGenericDOMException(InvalidCharacterError);
        return 0;
    }

    if (isXHTMLDocument() || isHTMLDocument())
        return HTMLElementFactory::createHTMLElement(QualifiedName(nullAtom, isHTMLDocument() ? name.lower() : name, xhtmlNamespaceURI), this, 0, false);

    return createElement(QualifiedName(nullAtom, name, nullAtom), false);
}
