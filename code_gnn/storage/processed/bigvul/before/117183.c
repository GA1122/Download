    ~InspectorTest()
    {
        g_signal_handlers_disconnect_matched(m_inspector, G_SIGNAL_MATCH_DATA, 0, 0, 0, 0, this);
    }
