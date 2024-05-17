WebPageProxy* WebInspectorProxy::platformCreateInspectorPage()
{
    ASSERT(m_page);
    ASSERT(!m_inspectorView);
    m_inspectorView = GTK_WIDGET(webkitWebViewBaseCreate(page()->process()->context(), inspectorPageGroup()));
    g_object_add_weak_pointer(G_OBJECT(m_inspectorView), reinterpret_cast<void**>(&m_inspectorView));
    return webkitWebViewBaseGetPage(WEBKIT_WEB_VIEW_BASE(m_inspectorView));
}
