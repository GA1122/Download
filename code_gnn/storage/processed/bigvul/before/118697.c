void V8WindowShell::updateDocument()
{
    ASSERT(m_world->isMainWorld());
    if (m_global.isEmpty())
        return;
    if (m_context.isEmpty())
        return;
    updateDocumentProperty();
    updateSecurityOrigin();
}
