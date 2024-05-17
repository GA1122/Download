PassRefPtr<HTMLCollection> Document::images()
{
    return ensureCachedCollection(DocImages);
}
