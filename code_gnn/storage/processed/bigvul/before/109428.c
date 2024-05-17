    BlobRegistryContext(const KURL& url, const KURL& srcURL)
        : url(url.copy())
        , srcURL(srcURL.copy())
    {
    }
