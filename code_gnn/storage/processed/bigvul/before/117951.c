void ScriptController::evaluateInIsolatedWorld(unsigned worldID, const Vector<ScriptSourceCode>& sources, int extensionGroup, Vector<ScriptValue>* results)
{
    v8::HandleScope handleScope;
    if (results) {
        Vector<v8::Local<v8::Value> > v8Results;
        m_proxy->evaluateInIsolatedWorld(worldID, sources, extensionGroup, &v8Results);
        Vector<v8::Local<v8::Value> >::iterator itr;
        for (itr = v8Results.begin(); itr != v8Results.end(); ++itr)
            results->append(ScriptValue(*itr));
    } else
        m_proxy->evaluateInIsolatedWorld(worldID, sources, extensionGroup, 0);
}
