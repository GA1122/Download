Frame* InspectorPageAgent::frameForId(const String& frameId)
{
    return frameId.isEmpty() ? 0 : m_identifierToFrame.get(frameId);
}
