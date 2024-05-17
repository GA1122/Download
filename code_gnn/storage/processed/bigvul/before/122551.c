void WebDevToolsAgentImpl::didProcessTask()
{
    if (!m_attached)
        return;
    if (InspectorController* ic = inspectorController())
        ic->didProcessTask();
    flushPendingFrontendMessages();
}
