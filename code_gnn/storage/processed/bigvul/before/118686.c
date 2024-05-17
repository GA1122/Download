void V8WindowShell::clearForClose(bool destroyGlobal)
{
    if (destroyGlobal)
        m_global.clear();

    if (m_context.isEmpty())
        return;

    m_document.clear();
    disposeContext();
}
