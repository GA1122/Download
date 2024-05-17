Response InspectorPageAgent::addScriptToEvaluateOnNewDocument(
    const String& source,
    String* identifier) {
  return addScriptToEvaluateOnLoad(source, identifier);
}
