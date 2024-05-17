void V8WindowShell::clearForNavigation()
{
    if (m_context.isEmpty())
        return;

    v8::HandleScope handleScope(m_isolate);
    m_document.clear();

    v8::Handle<v8::Context> context = m_context.newLocal(m_isolate);
    v8::Context::Scope contextScope(context);

    clearDocumentProperty();

    v8::Handle<v8::Object> windowWrapper = m_global.newLocal(m_isolate)->FindInstanceInPrototypeChain(V8Window::GetTemplate(m_isolate, worldTypeInMainThread(m_isolate)));
    ASSERT(!windowWrapper.IsEmpty());
    windowWrapper->TurnOnAccessCheck();
    context->DetachGlobal();
    disposeContext();
}
