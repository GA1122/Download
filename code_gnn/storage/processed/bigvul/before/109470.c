void BlobRegistryProxy::unregisterBlobURL(const KURL& url)
{
    if (m_webBlobRegistry)
        m_webBlobRegistry->unregisterBlobURL(url);
}
