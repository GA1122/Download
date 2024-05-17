void InspectorResourceAgent::didFailXHRLoading(XMLHttpRequest*, ThreadableLoaderClient* client)
{
    m_pendingXHRReplayData.remove(client);
}
