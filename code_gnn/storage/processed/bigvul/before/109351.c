void InspectorResourceAgent::didCloseWebSocket(Document*, unsigned long identifier)
{
    m_frontend->webSocketClosed(IdentifiersFactory::requestId(identifier), currentTime());
}
