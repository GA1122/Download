void ScriptController::evaluateInWorld(const ScriptSourceCode& source,
                                       DOMWrapperWorld* world)
{
    Vector<ScriptSourceCode> sources;
    sources.append(source);
    evaluateInIsolatedWorld(0, sources, 0);
}
