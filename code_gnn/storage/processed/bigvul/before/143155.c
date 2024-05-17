Element* Document::createElementNS(const AtomicString& namespaceURI, const AtomicString& qualifiedName, ExceptionState& exceptionState)
{
    QualifiedName qName(createQualifiedName(namespaceURI, qualifiedName, exceptionState));
    if (qName == QualifiedName::null())
        return nullptr;

    if (CustomElement::shouldCreateCustomElement(*this, qName))
        return CustomElement::createCustomElementSync(*this, qName, exceptionState);
    return createElement(qName, CreatedByCreateElement);
}
