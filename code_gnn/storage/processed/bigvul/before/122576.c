void WebDevToolsAgentImpl::reattach(const WebString& savedState)
{
    if (m_attached)
        return;

    inspectorController()->reuseFrontend(this, savedState);
    blink::Platform::current()->currentThread()->addTaskObserver(this);
    m_attached = true;
}
