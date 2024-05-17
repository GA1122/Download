void WebInspectorProxy::createInspectorWindow()
{
    if (m_client.openWindow(this))
        return;

    ASSERT(!m_inspectorWindow);
    m_inspectorWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    GtkWidget* inspectedViewParent = gtk_widget_get_toplevel(m_page->viewWidget());
    if (WebCore::widgetIsOnscreenToplevelWindow(inspectedViewParent))
        gtk_window_set_transient_for(GTK_WINDOW(m_inspectorWindow), GTK_WINDOW(inspectedViewParent));

    gtk_window_set_title(GTK_WINDOW(m_inspectorWindow), _("Web Inspector"));
    gtk_window_set_default_size(GTK_WINDOW(m_inspectorWindow), initialWindowWidth, initialWindowHeight);

    gtk_container_add(GTK_CONTAINER(m_inspectorWindow), m_inspectorView);
    gtk_widget_show(m_inspectorView);

    g_object_add_weak_pointer(G_OBJECT(m_inspectorWindow), reinterpret_cast<void**>(&m_inspectorWindow));
    gtk_window_present(GTK_WINDOW(m_inspectorWindow));
}
