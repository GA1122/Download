PassRefPtrWillBeRawPtr<DocumentNameCollection> Document::documentNamedItems(const AtomicString& name)
{
    return ensureCachedCollection<DocumentNameCollection>(DocumentNamedItems, name);
}
