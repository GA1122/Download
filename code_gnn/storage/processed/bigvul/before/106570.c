WebInspectorProxy* WebPageProxy::inspector()
{
    if (isClosed() || !isValid())
        return 0;
    if (!m_inspector)
        m_inspector = WebInspectorProxy::create(this);
    return m_inspector.get();
}
