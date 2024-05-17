    void showAndWaitUntilFinished(bool quitOnBringToFront)
    {
        m_quitOnBringToFront = quitOnBringToFront;
        webkit_web_inspector_show(m_inspector);
        g_main_loop_run(m_mainLoop);
        m_quitOnBringToFront = false;
    }
