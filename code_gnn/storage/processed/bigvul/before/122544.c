void WebDevToolsAgentImpl::detach()
{
    blink::Platform::current()->currentThread()->removeTaskObserver(this);

    InspectorController* ic = inspectorController();
    ic->disconnectFrontend();
    m_attached = false;
}
