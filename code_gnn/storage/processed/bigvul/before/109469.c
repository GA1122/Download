void BlobRegistryProxy::registerStreamURL(const KURL& url, const String& type)
{
    if (m_webBlobRegistry)
        m_webBlobRegistry->registerStreamURL(url, type);
}
