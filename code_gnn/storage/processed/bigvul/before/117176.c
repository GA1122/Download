    static gboolean detachIdle(InspectorTest* test)
    {
        webkit_web_inspector_detach(test->m_inspector);
        return FALSE;
    }
