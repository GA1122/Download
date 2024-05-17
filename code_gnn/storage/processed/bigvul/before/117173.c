    bool detach()
    {
        GRefPtr<WebKitWebViewBase> inspectorView = webkit_web_inspector_get_web_view(m_inspector);
        GtkWidget* pane = gtk_bin_get_child(GTK_BIN(m_parentWindow));
        g_assert(GTK_IS_PANED(pane));
        gtk_container_remove(GTK_CONTAINER(pane), GTK_WIDGET(inspectorView.get()));
        return InspectorTest::detach();
    }
