void DocumentThreadableLoader::loadActualRequest()
{
    ResourceRequest actualRequest = m_actualRequest;
    ResourceLoaderOptions actualOptions = m_actualOptions;
    m_actualRequest = ResourceRequest();
    m_actualOptions = ResourceLoaderOptions();

    actualRequest.setHTTPOrigin(getSecurityOrigin());

    clearResource();

    loadRequest(actualRequest, actualOptions);
}
