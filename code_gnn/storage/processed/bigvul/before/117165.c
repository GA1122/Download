    virtual bool bringToFront()
    {
        m_events.append(BringToFront);
        if (m_quitOnBringToFront)
            g_main_loop_quit(m_mainLoop);
        return FALSE;
    }
