PassRefPtrWillBeRawPtr<TagCollection> ContainerNode::getElementsByTagName(const AtomicString& localName)
{
    if (localName.isNull())
        return nullptr;

    if (document().isHTMLDocument())
        return ensureCachedCollection<HTMLTagCollection>(HTMLTagCollectionType, localName);
    return ensureCachedCollection<TagCollection>(TagCollectionType, localName);
}
