V8Proxy::V8Proxy(Frame* frame)
    : m_frame(frame)
    , m_windowShell(V8DOMWindowShell::create(frame))
{
}
