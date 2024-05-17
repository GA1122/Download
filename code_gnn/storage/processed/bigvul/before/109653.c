PassRefPtr<HTMLCollection> Document::documentNamedItems(const AtomicString& name)
{
    return ensureRareData()->ensureNodeLists()->addCacheWithAtomicName<HTMLNameCollection>(this, DocumentNamedItems, name);
}
