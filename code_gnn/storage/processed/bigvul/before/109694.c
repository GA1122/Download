bool Document::hasValidNamespaceForAttributes(const QualifiedName& qName)
{
    if (qName.prefix().isEmpty() && qName.localName() == xmlnsAtom) {
        return qName.namespaceURI() == XMLNSNames::xmlnsNamespaceURI;
    }
    return hasValidNamespaceForElements(qName);
}
