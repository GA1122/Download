void InspectorResourceAgent::willSendRequest(unsigned long identifier, DocumentLoader* loader, ResourceRequest& request, const ResourceResponse& redirectResponse, const FetchInitiatorInfo& initiatorInfo)
{
    if (initiatorInfo.name == FetchInitiatorTypeNames::internal)
        return;
    String requestId = IdentifiersFactory::requestId(identifier);
    m_resourcesData->resourceCreated(requestId, m_pageAgent->loaderId(loader));

    RefPtr<JSONObject> headers = m_state->getObject(ResourceAgentState::extraRequestHeaders);

    if (headers) {
        JSONObject::const_iterator end = headers->end();
        for (JSONObject::const_iterator it = headers->begin(); it != end; ++it) {
            String value;
            if (it->value->asString(&value))
                request.setHTTPHeaderField(it->key, value);
        }
    }

    request.setReportLoadTiming(true);
    request.setReportRawHeaders(true);

    if (m_state->getBoolean(ResourceAgentState::cacheDisabled)) {
        request.setHTTPHeaderField("Pragma", "no-cache");
        request.setCachePolicy(ReloadIgnoringCacheData);
        request.setHTTPHeaderField("Cache-Control", "no-cache");
    }

    String frameId = m_pageAgent->frameId(loader->frame());

    RefPtr<TypeBuilder::Network::Initiator> initiatorObject = buildInitiatorObject(loader->frame() ? loader->frame()->document() : 0, initiatorInfo);
    if (initiatorInfo.name == FetchInitiatorTypeNames::document) {
        FrameNavigationInitiatorMap::iterator it = m_frameNavigationInitiatorMap.find(frameId);
        if (it != m_frameNavigationInitiatorMap.end())
            initiatorObject = it->value;
    }

    m_frontend->requestWillBeSent(requestId, frameId, m_pageAgent->loaderId(loader), urlWithoutFragment(loader->url()).string(), buildObjectForResourceRequest(request), currentTime(), initiatorObject, buildObjectForResourceResponse(redirectResponse, loader));
}
