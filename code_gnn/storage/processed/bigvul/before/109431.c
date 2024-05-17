SecurityOrigin* BlobOriginCache::cachedOrigin(const KURL& url)
{
    if (url.protocolIs("blob"))
        return originMap()->get(url.string());
    return 0;
}
