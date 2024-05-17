    InspectorTest()
        : WebViewTest()
        , m_inspector(webkit_web_view_get_inspector(m_webView))
        , m_quitOnBringToFront(false)
    {
        webkit_settings_set_enable_developer_extras(webkit_web_view_get_settings(m_webView), TRUE);
        assertObjectIsDeletedWhenTestFinishes(G_OBJECT(m_inspector));
        g_signal_connect(m_inspector, "open-window", G_CALLBACK(openWindowCallback), this);
        g_signal_connect(m_inspector, "bring-to-front", G_CALLBACK(bringToFrontCallback), this);
        g_signal_connect(m_inspector, "closed", G_CALLBACK(closedCallback), this);
        g_signal_connect(m_inspector, "attach", G_CALLBACK(attachCallback), this);
        g_signal_connect(m_inspector, "detach", G_CALLBACK(detachCallback), this);
    }
