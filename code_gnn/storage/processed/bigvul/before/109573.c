PassRefPtr<HTMLCollection> Document::applets()
{
    return ensureCachedCollection(DocApplets);
}
