void WebDevToolsAgentImpl::attach()
{
    if (m_attached)
        return;

    inspectorController()->connectFrontend(this);
    inspectorController()->webViewResized(m_webViewImpl->size());
    blink::Platform::current()->currentThread()->addTaskObserver(this);
    m_attached = true;
}
