    static void didNavigate()
    {
        if (s_instance && s_instance->m_running)
            PageScriptDebugServer::shared().continueProgram();
    }
