PassRefPtrWillBeRawPtr<HTMLCollection> Document::images()
{
    return ensureCachedCollection<HTMLCollection>(DocImages);
}
