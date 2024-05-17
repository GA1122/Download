    void detachAndWaitUntilWindowOpened()
    {
        g_idle_add(reinterpret_cast<GSourceFunc>(detachIdle), this);
        g_main_loop_run(m_mainLoop);
    }
