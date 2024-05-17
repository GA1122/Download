void WebInspectorProxy::platformDidClose()
{
    if (m_inspectorView)
        g_signal_handlers_disconnect_by_func(m_inspectorView, reinterpret_cast<void*>(inspectorViewDestroyed), this);

    m_client.didClose(this);

    if (m_inspectorWindow) {
        gtk_widget_destroy(m_inspectorWindow);
        m_inspectorWindow = 0;
    }
    m_inspectorView = 0;
}
