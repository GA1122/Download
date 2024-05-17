PassRefPtr<Element> Document::createElementNS(const AtomicString& namespaceURI, const String& qualifiedName, const AtomicString& typeExtension, ExceptionCode& ec)
{
    String prefix, localName;
    if (!parseQualifiedName(qualifiedName, prefix, localName, ec))
        return 0;

    QualifiedName qName(prefix, localName, namespaceURI);
    if (!hasValidNamespaceForElements(qName)) {
        ec = NAMESPACE_ERR;
        return 0;
    }

    if (m_registry) {
        if (PassRefPtr<Element> created = m_registry->createElement(qName, typeExtension))
            return created;
    }

    return setTypeExtension(createElementNS(namespaceURI, qualifiedName, ec), typeExtension);
}
