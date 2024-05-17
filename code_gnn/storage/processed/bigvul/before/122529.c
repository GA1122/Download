void InspectorClientImpl::sendMessageToFrontend(PassRefPtr<WebCore::JSONObject> message)
{
    if (WebDevToolsAgentImpl* agent = devToolsAgent())
        agent->sendMessageToFrontend(message);
}
