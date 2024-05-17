void InspectorPageAgent::reload(ErrorString*, const bool* const optionalIgnoreCache, const String* optionalScriptToEvaluateOnLoad, const String* optionalScriptPreprocessor)
{
    m_pendingScriptToEvaluateOnLoadOnce = optionalScriptToEvaluateOnLoad ? *optionalScriptToEvaluateOnLoad : "";
    m_pendingScriptPreprocessor = optionalScriptPreprocessor ? *optionalScriptPreprocessor : "";
    m_page->mainFrame()->loader().reload(optionalIgnoreCache && *optionalIgnoreCache ? EndToEndReload : NormalReload);
}
