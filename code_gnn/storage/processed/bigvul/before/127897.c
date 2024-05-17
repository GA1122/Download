void DocumentThreadableLoader::dispatchInitialRequest(const ResourceRequest& request)
{
    if (m_sameOriginRequest || m_options.crossOriginRequestPolicy == AllowCrossOriginRequests) {
        loadRequest(request, m_resourceLoaderOptions);
        return;
    }

    ASSERT(m_options.crossOriginRequestPolicy == UseAccessControl);

    makeCrossOriginAccessRequest(request);
}
