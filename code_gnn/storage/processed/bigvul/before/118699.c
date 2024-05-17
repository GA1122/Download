void V8WindowShell::updateDocumentWrapper(v8::Handle<v8::Object> wrapper)
{
    ASSERT(m_world->isMainWorld());
    m_document.set(m_isolate, wrapper);
}
