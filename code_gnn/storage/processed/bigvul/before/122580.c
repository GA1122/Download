void WebDevToolsAgentImpl::sendMessageToFrontend(PassRefPtr<WebCore::JSONObject> message)
{
    m_frontendMessageQueue.append(message);
}
