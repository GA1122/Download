    void closeAndWaitUntilClosed()
    {
        webkit_web_inspector_close(m_inspector);
        g_main_loop_run(m_mainLoop);
    }
