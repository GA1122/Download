void V8Proxy::collectIsolatedContexts(Vector<std::pair<ScriptState*, SecurityOrigin*> >& result)
{
    v8::HandleScope handleScope;
    for (IsolatedWorldMap::iterator it = m_isolatedWorlds.begin(); it != m_isolatedWorlds.end(); ++it) {
        V8IsolatedContext* isolatedContext = it->second;
        if (!isolatedContext->securityOrigin())
            continue;
        v8::Handle<v8::Context> v8Context = isolatedContext->context();
        if (v8Context.IsEmpty())
            continue;
        ScriptState* scriptState = ScriptState::forContext(v8::Local<v8::Context>::New(v8Context));
        result.append(std::pair<ScriptState*, SecurityOrigin*>(scriptState, isolatedContext->securityOrigin()));
    }
}
