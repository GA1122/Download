    void executeScript(const WebString& code)
    {
        m_frame->executeScript(WebScriptSource(code));
        runPendingTasks();
    }
