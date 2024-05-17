PassRefPtr<Element> Document::createElementNS(const String& namespaceURI, const String& qualifiedName, ExceptionState& es)
{
    String prefix, localName;
    if (!parseQualifiedName(qualifiedName, prefix, localName, es))
        return 0;

    QualifiedName qName(prefix, localName, namespaceURI);
    if (!hasValidNamespaceForElements(qName)) {
        es.throwUninformativeAndGenericDOMException(NamespaceError);
        return 0;
    }

    return createElement(qName, false);
}
