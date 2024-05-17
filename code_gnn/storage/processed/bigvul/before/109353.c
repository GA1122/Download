void InspectorResourceAgent::didCreateWebSocket(Document*, unsigned long identifier, const KURL& requestURL, const String&)
{
    m_frontend->webSocketCreated(IdentifiersFactory::requestId(identifier), urlWithoutFragment(requestURL).string());
}
