Response InspectorPageAgent::removeScriptToEvaluateOnLoad(
    const String& identifier) {
  protocol::DictionaryValue* scripts =
      state_->getObject(PageAgentState::kPageAgentScriptsToEvaluateOnLoad);
  if (!scripts || !scripts->get(identifier))
    return Response::Error("Script not found");
  scripts->remove(identifier);
  return Response::OK();
}
