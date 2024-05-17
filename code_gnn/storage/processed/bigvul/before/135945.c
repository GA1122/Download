PassRefPtrWillBeRawPtr<TagCollection> ContainerNode::getElementsByTagNameNS(const AtomicString& namespaceURI, const AtomicString& localName)
{
    if (localName.isNull())
        return nullptr;

    if (namespaceURI == starAtom)
        return getElementsByTagName(localName);

    return ensureCachedCollection<TagCollection>(TagCollectionType, namespaceURI.isEmpty() ? nullAtom : namespaceURI, localName);
}
