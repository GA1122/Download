void InspectorOverlay::evaluateInOverlay(const String& method, const String& argument)
{
    RefPtr<JSONArray> command = JSONArray::create();
    command->pushString(method);
    command->pushString(argument);
    overlayPage()->mainFrame()->script().executeScriptInMainWorld("dispatch(" + command->toJSONString() + ")", ScriptController::ExecuteScriptWhenScriptsDisabled);
}
