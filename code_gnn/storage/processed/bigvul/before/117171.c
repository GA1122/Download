    void destroyWindowAndWaitUntilClosed()
    {
        g_assert(m_inspectorWindow);
        gtk_widget_destroy(m_inspectorWindow);
        m_inspectorWindow = 0;
        g_main_loop_run(m_mainLoop);
    }
