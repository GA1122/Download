void BlobRegistryProxy::addDataToStream(const KURL& url, PassRefPtr<RawData> streamData)
{
    if (m_webBlobRegistry) {
        WebKit::WebThreadSafeData webThreadSafeData(streamData);
        m_webBlobRegistry->addDataToStream(url, webThreadSafeData);
    }
}
