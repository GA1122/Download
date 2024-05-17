static inline QualifiedName createQualifiedName(const AtomicString& namespaceURI, const AtomicString& qualifiedName, ExceptionState& exceptionState)
{
    AtomicString prefix, localName;
    if (!Document::parseQualifiedName(qualifiedName, prefix, localName, exceptionState))
        return QualifiedName::null();

    QualifiedName qName(prefix, localName, namespaceURI);
    if (!Document::hasValidNamespaceForElements(qName)) {
        exceptionState.throwDOMException(NamespaceError, "The namespace URI provided ('" + namespaceURI + "') is not valid for the qualified name provided ('" + qualifiedName + "').");
        return QualifiedName::null();
    }

    return qName;
}
