PassRefPtrWillBeRawPtr<HTMLAllCollection> Document::all()
{
    return ensureCachedCollection<HTMLAllCollection>(DocAll);
}
