    ReadResultCapturingFunction(ScriptState* scriptState, ReadResult* value)
        : ScriptFunction(scriptState)
        , m_result(value)
    {
    }
