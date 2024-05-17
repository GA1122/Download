void V8WindowShell::clearDocumentProperty()
{
    ASSERT(!m_context.isEmpty());
    if (!m_world->isMainWorld())
        return;
    v8::HandleScope handleScope(m_isolate);
    m_context.newLocal(m_isolate)->Global()->ForceDelete(v8::String::NewSymbol("document"));
}
