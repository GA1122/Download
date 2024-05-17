void ScriptController::collectIsolatedContexts(Vector<std::pair<ScriptState*, SecurityOrigin*> >& result)
{
    m_proxy->collectIsolatedContexts(result);
}
