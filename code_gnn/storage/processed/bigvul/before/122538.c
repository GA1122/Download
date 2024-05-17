void InspectorClientImpl::updateInspectorStateCookie(const WTF::String& inspectorState)
{
    if (WebDevToolsAgentImpl* agent = devToolsAgent())
         agent->updateInspectorStateCookie(inspectorState);
 }
