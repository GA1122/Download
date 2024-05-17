ScriptController::ScriptController(Frame* frame)
    : m_frame(frame)
    , m_sourceURL(0)
    , m_paused(false)
    , m_proxy(adoptPtr(new V8Proxy(frame)))
#if ENABLE(NETSCAPE_PLUGIN_API)
    , m_wrappedWindowScriptNPObject(0)
#endif
{
}
