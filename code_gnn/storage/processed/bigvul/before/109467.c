void BlobRegistryProxy::registerBlobURL(const KURL& url, PassOwnPtr<BlobData> blobData)
{
    if (m_webBlobRegistry) {
        WebKit::WebBlobData webBlobData(blobData);
        m_webBlobRegistry->registerBlobURL(url, webBlobData);
    }
}
