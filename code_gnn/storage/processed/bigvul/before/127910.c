void DocumentThreadableLoader::makeCrossOriginAccessRequest(const ResourceRequest& request)
{
    ASSERT(m_options.crossOriginRequestPolicy == UseAccessControl);
    ASSERT(m_client);
    ASSERT(!resource());

    if (!SchemeRegistry::shouldTreatURLSchemeAsCORSEnabled(request.url().protocol())) {
        InspectorInstrumentation::documentThreadableLoaderFailedToStartLoadingForClient(m_document, m_client);
        ThreadableLoaderClient* client = m_client;
        clear();
        client->didFailAccessControlCheck(ResourceError(errorDomainBlinkInternal, 0, request.url().getString(), "Cross origin requests are only supported for protocol schemes: " + SchemeRegistry::listOfCORSEnabledURLSchemes() + "."));
        return;
    }

    if ((m_options.preflightPolicy == ConsiderPreflight && FetchUtils::isSimpleOrForbiddenRequest(request.httpMethod(), request.httpHeaderFields())) || m_options.preflightPolicy == PreventPreflight) {
        ResourceRequest crossOriginRequest(request);
        ResourceLoaderOptions crossOriginOptions(m_resourceLoaderOptions);
        updateRequestForAccessControl(crossOriginRequest, getSecurityOrigin(), effectiveAllowCredentials());
        crossOriginRequest.setFetchCredentialsMode(effectiveAllowCredentials() == AllowStoredCredentials ? WebURLRequest::FetchCredentialsModeInclude : WebURLRequest::FetchCredentialsModeOmit);
        loadRequest(crossOriginRequest, crossOriginOptions);
    } else {
        m_crossOriginNonSimpleRequest = true;

        ResourceRequest crossOriginRequest(request);
        ResourceLoaderOptions crossOriginOptions(m_resourceLoaderOptions);
        updateRequestForAccessControl(crossOriginRequest, 0, effectiveAllowCredentials());
        crossOriginRequest.setFetchCredentialsMode(effectiveAllowCredentials() == AllowStoredCredentials ? WebURLRequest::FetchCredentialsModeInclude : WebURLRequest::FetchCredentialsModeOmit);
        m_actualRequest = crossOriginRequest;
        m_actualOptions = crossOriginOptions;

        bool shouldForcePreflight = InspectorInstrumentation::shouldForceCORSPreflight(m_document);
        bool canSkipPreflight = CrossOriginPreflightResultCache::shared().canSkipPreflight(getSecurityOrigin()->toString(), m_actualRequest.url(), effectiveAllowCredentials(), m_actualRequest.httpMethod(), m_actualRequest.httpHeaderFields());
        if (canSkipPreflight && !shouldForcePreflight) {
            loadActualRequest();
        } else {
            ResourceRequest preflightRequest = createAccessControlPreflightRequest(m_actualRequest, getSecurityOrigin());
            ResourceLoaderOptions preflightOptions = m_actualOptions;
            preflightOptions.allowCredentials = DoNotAllowStoredCredentials;
            loadRequest(preflightRequest, preflightOptions);
        }
    }
}
