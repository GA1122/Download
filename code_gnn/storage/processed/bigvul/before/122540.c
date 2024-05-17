    ClientMessageLoopAdapter(PassOwnPtr<blink::WebDevToolsAgentClient::WebKitClientMessageLoop> messageLoop)
        : m_running(false)
        , m_messageLoop(messageLoop) { }
