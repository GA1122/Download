void V8WindowShell::disposeContext()
{
    m_perContextData.clear();

    if (m_context.isEmpty())
        return;

    v8::HandleScope handleScope(m_isolate);
    m_frame->loader()->client()->willReleaseScriptContext(m_context.newLocal(m_isolate), m_world->worldId());

    m_context.clear();

    bool isMainFrame = m_frame->page() && (m_frame->page()->mainFrame() == m_frame);
    V8GCForContextDispose::instance().notifyContextDisposed(isMainFrame);
}
