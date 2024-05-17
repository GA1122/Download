v8::Local<v8::Context> V8Proxy::isolatedWorldContext(int worldId)
{
    IsolatedWorldMap::iterator iter = m_isolatedWorlds.find(worldId);
    if (iter == m_isolatedWorlds.end())
        return v8::Local<v8::Context>();
    return v8::Local<v8::Context>::New(iter->second->context());
}
