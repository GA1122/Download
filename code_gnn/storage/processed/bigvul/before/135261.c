bool Document::hasValidNamespaceForElements(const QualifiedName& qName)
{
    if (!qName.prefix().isEmpty() && qName.namespaceURI().isNull())  
        return false;
    if (qName.prefix() == xmlAtom && qName.namespaceURI() != XMLNames::xmlNamespaceURI)  
        return false;

    if (qName.prefix() == xmlnsAtom || (qName.prefix().isEmpty() && qName.localName() == xmlnsAtom))
        return qName.namespaceURI() == XMLNSNames::xmlnsNamespaceURI;
    return qName.namespaceURI() != XMLNSNames::xmlnsNamespaceURI;
}
