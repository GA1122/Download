PassRefPtrWillBeRawPtr<Attr> Document::createAttributeNS(const AtomicString& namespaceURI, const AtomicString& qualifiedName, ExceptionState& exceptionState, bool shouldIgnoreNamespaceChecks)
{
    AtomicString prefix, localName;
    if (!parseQualifiedName(qualifiedName, prefix, localName, exceptionState))
        return nullptr;

    QualifiedName qName(prefix, localName, namespaceURI);

    if (!shouldIgnoreNamespaceChecks && !hasValidNamespaceForAttributes(qName)) {
        exceptionState.throwDOMException(NamespaceError, "The namespace URI provided ('" + namespaceURI + "') is not valid for the qualified name provided ('" + qualifiedName + "').");
        return nullptr;
    }

    return Attr::create(*this, qName, emptyAtom);
}
