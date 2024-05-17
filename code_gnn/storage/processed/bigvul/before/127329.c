    void loadHTML(const WebData& html)
    {
        m_frame->loadHTMLString(html, toKURL("about:blank"));
        runPendingTasks();
    }
