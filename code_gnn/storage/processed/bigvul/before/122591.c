void WebDevToolsAgentImpl::willProcessTask()
{
    if (!m_attached)
        return;
    if (InspectorController* ic = inspectorController())
        ic->willProcessTask();
}
