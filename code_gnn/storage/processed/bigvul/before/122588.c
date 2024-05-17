void WebDevToolsAgentImpl::updateInspectorStateCookie(const String& state)
{
     m_client->saveAgentRuntimeState(state);
 }
