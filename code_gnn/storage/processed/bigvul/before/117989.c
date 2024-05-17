void V8Proxy::evaluateInIsolatedWorld(int worldID, const Vector<ScriptSourceCode>& sources, int extensionGroup, WTF::Vector<v8::Local<v8::Value> >* results)
{
    if (!windowShell()->initContextIfNeeded())
        return;

    v8::HandleScope handleScope;
    V8IsolatedContext* isolatedContext = 0;

    if (worldID > 0) {
        IsolatedWorldMap::iterator iter = m_isolatedWorlds.find(worldID);
        if (iter != m_isolatedWorlds.end()) {
            isolatedContext = iter->second;
        } else {
            isolatedContext = new V8IsolatedContext(this, extensionGroup, worldID);
            if (isolatedContext->context().IsEmpty()) {
                delete isolatedContext;
                return;
            }

            m_isolatedWorlds.set(worldID, isolatedContext);
        }

        IsolatedWorldSecurityOriginMap::iterator securityOriginIter = m_isolatedWorldSecurityOrigins.find(worldID);
        if (securityOriginIter != m_isolatedWorldSecurityOrigins.end())
            isolatedContext->setSecurityOrigin(securityOriginIter->second);
    } else {
        isolatedContext = new V8IsolatedContext(this, extensionGroup, worldID);
        if (isolatedContext->context().IsEmpty()) {
            delete isolatedContext;
            return;
        }
    }

    v8::Local<v8::Context> context = v8::Local<v8::Context>::New(isolatedContext->context());
    v8::Context::Scope context_scope(context);

    if (results) {
        for (size_t i = 0; i < sources.size(); ++i)
            results->append(evaluate(sources[i], 0));
    } else {
        for (size_t i = 0; i < sources.size(); ++i)
            evaluate(sources[i], 0);
    }

    if (worldID == 0)
        isolatedContext->destroy();
}
