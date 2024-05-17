    virtual void closed()
    {
        m_events.append(Closed);
        g_main_loop_quit(m_mainLoop);
    }
