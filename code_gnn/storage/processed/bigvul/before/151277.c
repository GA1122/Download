Response InspectorPageAgent::removeScriptToEvaluateOnNewDocument(
    const String& identifier) {
  return removeScriptToEvaluateOnLoad(identifier);
}
