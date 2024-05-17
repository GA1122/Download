    bool openWindow()
    {
        g_assert(!m_inspectorWindow);
        m_inspectorWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
        WebKitWebViewBase* inspectorView = webkit_web_inspector_get_web_view(m_inspector);
        g_assert(inspectorView);
        gtk_container_add(GTK_CONTAINER(m_inspectorWindow), GTK_WIDGET(inspectorView));
        gtk_widget_show_all(m_inspectorWindow);

        return InspectorTest::openWindow();
    }
