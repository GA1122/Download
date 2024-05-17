void WebPageProxy::initializeResourceLoadClient(const WKPageResourceLoadClient* client)
{
    m_resourceLoadClient.initialize(client);
}
