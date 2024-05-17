void ScriptController::evaluateInIsolatedWorld(unsigned worldID, const Vector<ScriptSourceCode>& sources, Vector<ScriptValue>* results)
{
    evaluateInIsolatedWorld(worldID, sources, 0, results);
}
