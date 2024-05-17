PassRefPtrWillBeRawPtr<HTMLCollection> Document::windowNamedItems(const AtomicString& name)
{
    return ensureCachedCollection<WindowNameCollection>(WindowNamedItems, name);
}
