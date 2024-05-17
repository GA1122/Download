PassRefPtrWillBeRawPtr<HTMLCollection> Document::applets()
{
    return ensureCachedCollection<HTMLCollection>(DocApplets);
}
