v8::Local<v8::Value> PrivateScriptRunner::installClassIfNeeded(Document* document, String className)
{
    if (!document->contextDocument()->frame())
        return v8::Local<v8::Value>();

    v8::HandleScope handleScope(toIsolate(document));
    ScriptState* scriptState = ScriptState::forWorld(document->contextDocument()->frame(), DOMWrapperWorld::privateScriptIsolatedWorld());
    if (!scriptState)
        return v8::Local<v8::Value>();

    ScriptState::Scope scope(scriptState);
    return classObjectOfPrivateScript(scriptState, className);
}
