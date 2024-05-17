bool V8Proxy::matchesCurrentContext()
{
    v8::Handle<v8::Context> context;
    if (V8IsolatedContext* isolatedContext = V8IsolatedContext::getEntered()) {
        context = isolatedContext->sharedContext()->get();
        if (m_frame != V8Proxy::retrieveFrame(context))
            return false;
    } else {
        windowShell()->initContextIfNeeded();
        context = windowShell()->context();
    }
    return context == context->GetCurrent();
}
