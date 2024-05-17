PassRefPtr<HTMLCollection> Document::scripts()
{
    return ensureCachedCollection(DocScripts);
}
