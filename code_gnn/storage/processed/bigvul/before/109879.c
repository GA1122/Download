PassRefPtr<HTMLCollection> Document::windowNamedItems(const AtomicString& name)
{
    return ensureRareData()->ensureNodeLists()->addCacheWithAtomicName<HTMLNameCollection>(this, WindowNamedItems, name);
}
