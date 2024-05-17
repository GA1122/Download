    ~CustomInspectorTest()
    {
        if (m_inspectorWindow)
            gtk_widget_destroy(m_inspectorWindow);
    }
