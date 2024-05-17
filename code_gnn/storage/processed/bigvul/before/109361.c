void InspectorResourceAgent::didFinishXHRLoading(ThreadableLoaderClient* client, unsigned long identifier, ScriptString sourceString, const String&, const String&, unsigned)
{
    m_pendingXHRReplayData.remove(client);
}
