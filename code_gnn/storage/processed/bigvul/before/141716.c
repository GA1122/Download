V8Debugger::V8Debugger(v8::Isolate* isolate, V8InspectorImpl* inspector)
    : m_isolate(isolate)
    , m_inspector(inspector)
    , m_lastContextId(0)
    , m_enableCount(0)
    , m_breakpointsActivated(true)
    , m_runningNestedMessageLoop(false)
    , m_ignoreScriptParsedEventsCounter(0)
    , m_maxAsyncCallStackDepth(0)
{
}
