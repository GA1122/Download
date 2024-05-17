    void closed()
    {
        if (m_inspectorWindow) {
            gtk_widget_destroy(m_inspectorWindow);
            m_inspectorWindow = 0;
        }

        return InspectorTest::closed();
    }
