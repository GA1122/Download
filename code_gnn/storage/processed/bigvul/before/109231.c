void InspectorOverlay::evaluateInOverlay(const String& method, PassRefPtr<JSONValue> argument)
{
    RefPtr<JSONArray> command = JSONArray::create();
    command->pushString(method);
    command->pushValue(argument);
    overlayPage()->mainFrame()->script().executeScriptInMainWorld("dispatch(" + command->toJSONString() + ")", ScriptController::ExecuteScriptWhenScriptsDisabled);
}
