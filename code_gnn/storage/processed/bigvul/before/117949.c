ScriptValue ScriptController::evaluate(const ScriptSourceCode& sourceCode)
{
    String sourceURL = sourceCode.url();
    const String* savedSourceURL = m_sourceURL;
    m_sourceURL = &sourceURL;

    v8::HandleScope handleScope;
    v8::Handle<v8::Context> v8Context = V8Proxy::mainWorldContext(m_proxy->frame());
    if (v8Context.IsEmpty())
        return ScriptValue();

    v8::Context::Scope scope(v8Context);

    RefPtr<Frame> protect(m_frame);

    v8::Local<v8::Value> object = m_proxy->evaluate(sourceCode, 0);

    m_sourceURL = savedSourceURL;

    if (object.IsEmpty())
        return ScriptValue();

    return ScriptValue(object);
}
