    V8InspectorClient* ensureDebuggerClient()
    {
        if (m_inspectorClient)
            return m_inspectorClient;
        InspectedContext* inspectedContext = ensureInspectedContext();
        if (!inspectedContext)
            return nullptr;
        m_inspectorClient = inspectedContext->inspector()->client();
        return m_inspectorClient;
    }
