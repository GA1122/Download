    BlobRegistryContext(const KURL& url, const String& type)
        : url(url.copy())
        , type(type.isolatedCopy())
    {
    }
