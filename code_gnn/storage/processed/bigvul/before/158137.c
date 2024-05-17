String LocalFrameClientImpl::evaluateInInspectorOverlayForTesting(
    const String& script) {
  if (WebDevToolsAgentImpl* devtools = DevToolsAgent())
    return devtools->EvaluateInOverlayForTesting(script);
  return g_empty_string;
}
