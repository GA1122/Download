PassRefPtr<HTMLCollection> Document::all()
{
    return ensureRareData()->ensureNodeLists()->addCacheWithAtomicName<HTMLAllCollection>(this, DocAll);
}
