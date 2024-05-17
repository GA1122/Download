void WebInspectorProxy::platformInspectedURLChanged(const String& url)
{
    m_client.inspectedURLChanged(this, url);

    if (!m_inspectorWindow)
        return;
    GOwnPtr<gchar> title(g_strdup_printf("%s - %s", _("Web Inspector"), url.utf8().data()));
    gtk_window_set_title(GTK_WINDOW(m_inspectorWindow), title.get());
}
