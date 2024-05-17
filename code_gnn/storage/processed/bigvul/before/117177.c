    virtual bool openWindow()
    {
        m_events.append(OpenWindow);
        g_main_loop_quit(m_mainLoop);
        return TRUE;
    }
