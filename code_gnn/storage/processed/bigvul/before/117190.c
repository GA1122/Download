void WebInspectorProxy::platformBringToFront()
{
    if (m_client.bringToFront(this))
        return;

    GtkWidget* parent = gtk_widget_get_toplevel(m_inspectorView);
    if (WebCore::widgetIsOnscreenToplevelWindow(parent))
        gtk_window_present(GTK_WINDOW(parent));
}
