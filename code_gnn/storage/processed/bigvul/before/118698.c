void V8WindowShell::updateDocumentProperty()
{
    if (!m_world->isMainWorld())
        return;

    v8::HandleScope handleScope(m_isolate);
    v8::Handle<v8::Context> context = m_context.newLocal(m_isolate);
    v8::Context::Scope contextScope(context);

    v8::Handle<v8::Value> documentWrapper = toV8(m_frame->document(), v8::Handle<v8::Object>(), context->GetIsolate());
    ASSERT(documentWrapper == m_document.newLocal(m_isolate) || m_document.isEmpty());
    if (m_document.isEmpty())
        updateDocumentWrapper(v8::Handle<v8::Object>::Cast(documentWrapper));
    checkDocumentWrapper(m_document.newLocal(m_isolate), m_frame->document());

    if (documentWrapper.IsEmpty()) {
        clearDocumentProperty();
        return;
    }
    ASSERT(documentWrapper->IsObject());
    context->Global()->ForceSet(v8::String::NewSymbol("document"), documentWrapper, static_cast<v8::PropertyAttribute>(v8::ReadOnly | v8::DontDelete));

    toInnerGlobalObject(context)->SetHiddenValue(V8HiddenPropertyName::document(m_isolate), documentWrapper);
}
