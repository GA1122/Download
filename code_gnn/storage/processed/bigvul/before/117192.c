void WebInspectorProxy::platformDetach()
{
    if (!m_page->isValid())
        return;

    GRefPtr<GtkWidget> inspectorView = m_inspectorView;
    if (!m_client.detach(this)) {
        GtkWidget* parent = gtk_widget_get_parent(m_inspectorView);
        ASSERT(parent);
        gtk_container_remove(GTK_CONTAINER(parent), m_inspectorView);
    }

    if (!m_isVisible)
        return;

    createInspectorWindow();
}
