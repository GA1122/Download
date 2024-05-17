PassRefPtr<HTMLCollection> Document::anchors()
{
    return ensureCachedCollection(DocAnchors);
}
