    V8DebuggerAgentImpl* debuggerAgent()
    {
        if (V8InspectorSessionImpl* session = currentSession()) {
            if (session && session->debuggerAgent()->enabled())
                return session->debuggerAgent();
        }
        return nullptr;
    }
