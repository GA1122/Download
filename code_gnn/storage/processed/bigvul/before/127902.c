void DocumentThreadableLoader::handlePreflightFailure(const String& url, const String& errorDescription)
{
    ResourceError error(errorDomainBlinkInternal, 0, url, errorDescription);

    m_actualRequest = ResourceRequest();

    ThreadableLoaderClient* client = m_client;
    clear();
    client->didFailAccessControlCheck(error);
}
