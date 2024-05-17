    V8ProfilerAgentImpl* profilerAgent()
    {
        if (V8InspectorSessionImpl* session = currentSession()) {
            if (session && session->profilerAgent()->enabled())
                return session->profilerAgent();
        }
        return nullptr;
    }
