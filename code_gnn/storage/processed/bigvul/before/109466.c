void BlobRegistryProxy::finalizeStream(const KURL& url)
{
    if (m_webBlobRegistry)
        m_webBlobRegistry->finalizeStream(url);
}
