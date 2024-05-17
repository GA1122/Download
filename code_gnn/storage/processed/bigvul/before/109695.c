bool Document::hasValidNamespaceForElements(const QualifiedName& qName)
{
    if (!qName.prefix().isEmpty() && qName.namespaceURI().isNull())  
        return false;
    if (qName.prefix() == xmlAtom && qName.namespaceURI() != XMLNames::xmlNamespaceURI)  
        return false;

    if ((qName.prefix() == xmlnsAtom && qName.namespaceURI() != XMLNSNames::xmlnsNamespaceURI) || (qName.prefix() != xmlnsAtom && qName.namespaceURI() == XMLNSNames::xmlnsNamespaceURI))
        return false;

    return true;
}
